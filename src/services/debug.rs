use std::sync::{Arc, Mutex};

use crate::hyprland::listener::HyprlandListener;

pub struct DebugService;

impl DebugService {
  pub fn new(listener: Arc<Mutex<HyprlandListener>>) {
    if let Ok(mut guard) = listener.lock() {
      guard.subscribe(Box::new(|event| {
        println!("[Debug] Event Received: {:?}", event);
      }));
      println!("[DebugService] Subscribed to listener.");
    }
  }
}
