use std::sync::{Arc, Mutex, MutexGuard};

use serde::Deserialize;

use crate::{
  hyprland::{hyprland_event::HyprlandEvent, hyprland_ipc::HyprlandIpc},
  shared::LockError,
};

#[derive(Deserialize, Default, Debug, Clone)]
struct Window {
  pub address: String,
  pub class: String,
  pub title: String,
}

#[derive(Default, Debug)]
struct WindowState {
  active_window: String,
  all_windows: Vec<Window>,
}

impl WindowState {
  pub fn new() -> WindowState {
    let mut initial_state = WindowState::default();

    println!("[WindowService] Getting current windows...");

    match HyprlandIpc::send_command("j/clients") {
      Ok(response) => {
        initial_state.all_windows = serde_json::from_str(&response).unwrap_or_default();
        print!("[WindowService] Initial {initial_state:?}");
      },
      Err(e) => {
        eprintln!("[WindowService] Failed to get initial workspace {e}");
      },
    }

    initial_state
  }

  fn on_event(&mut self, event: HyprlandEvent) {
    match event.name.as_str() {
      "activewindowv2" => {
        println!("[WindowService] Active window changed {}", event.data);
        self.active_window = event.data;
      },
      "openwindow" => {
        let parts: Vec<&str> = event.data.splitn(4, ',').collect();

        if let [address, _, class, title] = &parts[..] {
          self.all_windows.push(Window {
            address: address.to_string(),
            class: class.to_string(),
            title: title.to_string(),
          });
        } else {
          eprintln!("[WindowService] openwindow: couldn't figure out the event data {parts:?}")
        }
      },
      "closewindow" => {
        self.all_windows.retain(|w| w.address != event.data);
      },
      _ => (),
    }
  }
}

pub struct WindoService {
  state: Arc<Mutex<WindowState>>,
}

impl WindoService {
  pub fn new(listener: Arc<Mutex<HyprlandIpc>>) -> Self {
    let state = Arc::new(Mutex::new(WindowState::new()));

    let state_clone = Arc::clone(&state);
    if let Ok(mut listen) = listener.lock() {
      listen.subscribe(Box::new(move |event| {
        if let Ok(mut st) = state_clone.lock() {
          st.on_event(event);
        }
      }));
    }

    Self { state }
  }

  fn get_state<P, R>(&self, getter: P) -> Result<R, LockError>
  where
    P: FnOnce(MutexGuard<WindowState>) -> R,
  {
    match self.state.lock() {
      Ok(guard) => Ok(getter(guard)),
      Err(e) => Err(format!("Failed to acquire lock: {}", e)),
    }
  }

  pub fn get_active_window(&self) -> Result<String, LockError> {
    self.get_state(|guard| guard.active_window.clone())
  }
}
