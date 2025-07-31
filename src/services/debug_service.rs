use std::sync::{Arc, Mutex};

use crate::hyprland::hyprland_ipc::HyprlandIpc;

pub struct DebugService;

impl DebugService {
  pub fn new(listener: Arc<Mutex<HyprlandIpc>>) -> Self {
    if let Ok(mut guard) = listener.lock() {
      guard.subscribe(Box::new(|event| {
        println!("[Debug] Event Received: {event:?}");
      }));
      println!("[DebugService] Subscribed to listener.");
    }

    Self
  }
}
