use std::sync::{Arc, Mutex};

use crate::hyprland::hyprland_listener::HyprlandListener;

#[derive(Default, Debug)]
struct WorkspaceState {
  active_workspace: String,
}

pub struct WorkspaceService {
  _state: Arc<Mutex<WorkspaceState>>,
}

impl WorkspaceService {
  pub fn new(listener: Arc<Mutex<HyprlandListener>>) -> Self {
    let state = Arc::new(Mutex::new(WorkspaceState::default()));
    let state_clone = Arc::clone(&state);

    if let Ok(mut listen) = listener.lock() {
      listen.subscribe(Box::new(move |event| {
        if event.name == "workspace" {
          if let Ok(mut st) = state_clone.lock() {
            st.active_workspace = event.data;
            println!("[WorkspaceService] Active workspace {st:?}");
          }
        }
      }));
    }

    Self { _state: state }
  }
}
