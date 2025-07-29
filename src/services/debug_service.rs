use crate::hyprland_listener::HyprlandListener;
use std::sync::{Arc, Mutex};

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
