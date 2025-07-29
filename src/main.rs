use std::sync::{Arc, Mutex};

use crate::{hyprland::listener::HyprlandListener, services::debug::DebugService};

mod hyprland;
mod services;

fn main() {
  let listener = Arc::new(Mutex::new(HyprlandListener::default()));

  if let Ok(mut listen) = listener.lock() {
    if let Err(e) = listen.connect() {
      eprintln!("[HyprlandListener] Failed to connect: {e}");
      return;
    }
  }

  DebugService::new(Arc::clone(&listener));

  // This increases the reference count.
  // This does not clone the listener itself. It only clones the Arc pointer,
  let handle = HyprlandListener::start(Arc::clone(&listener));
  println!("Main thread continues...");

  handle.join().expect("Listener thread paicked");
}
