use std::sync::{Arc, Mutex};

use serde::Deserialize;

use crate::hyprland::{hyprland_event::HyprlandEvent, hyprland_ipc::HyprlandIpc};

#[derive(Deserialize, Debug, Clone)]
pub struct Workspace {
  pub id: i32,
  pub name: String,
}

#[derive(Default, Debug)]
struct WorkspaceState {
  active_workspace: String,
  all_workspaces: Vec<Workspace>,
}

pub struct WorkspaceService {
  state: Arc<Mutex<WorkspaceState>>,
}

type LockError = String;

impl WorkspaceService {
  pub fn new(listener: Arc<Mutex<HyprlandIpc>>) -> Self {
    let state = Arc::new(Mutex::new(WorkspaceState::new()));

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
  pub fn get_active_workspace(&self) -> Result<String, LockError> {
    return match self.state.lock() {
      Ok(guard) => Ok(guard.active_workspace.clone()),
      Err(e) => Err(format!("Failed to acquire lock: {}", e)),
    };
  }

  pub fn get_all_workspaces(&self) -> Result<Vec<Workspace>, LockError> {
    return match self.state.lock() {
      Ok(guard) => Ok(guard.all_workspaces.clone()),
      Err(e) => Err(format!("Failed to acquire lock: {}", e)),
    };
  }
}

impl WorkspaceState {
  pub fn new() -> WorkspaceState {
    let mut initial_state = WorkspaceState::default();

    println!("[WorkspaceService] Fetching workspaces...");

    match HyprlandIpc::send_command("j/workspaces") {
      Ok(response) => {
        initial_state.all_workspaces = serde_json::from_str(&response).unwrap_or_default();
        println!("[WorkspaceService] Intial {initial_state:?}");
      },
      Err(e) => {
        eprintln!("[WorkspaceService] Failed to get initial workspaces {e}");
      },
    }

    initial_state
  }

  fn on_event(&mut self, event: HyprlandEvent) {
    match event.name.as_str() {
      "workspace" => {
        println!("[WorkspaceService] Active workspace {}", event.data);
        self.active_workspace = event.data;
      },
      "createworkspacev2" => {
        if let Some((id_str, name)) = event.data.split_once(',') {
          if let Ok(id) = id_str.parse::<i32>() {
            self.all_workspaces.push(Workspace { id, name: name.to_string() });
          }
        }
      },
      "destroyworkspacev2" => {
        if let Some((id_str, _)) = event.data.split_once(',') {
          if let Ok(id) = id_str.parse::<i32>() {
            self.all_workspaces.retain(|ws| ws.id != id);
          }
        }
      },
      _ => (),
    }
  }
}
