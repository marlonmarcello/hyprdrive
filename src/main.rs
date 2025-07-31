use hyprland::*;
use services::*;
use std::sync::{Arc, Mutex};

use crate::{debug_service::DebugService, hyprland_ipc::HyprlandIpc, workspace_service::WorkspaceService};

mod hyprland;
mod services;

fn main() {
  let listener = Arc::new(Mutex::new(HyprlandIpc::default()));

  if let Ok(mut listen) = listener.lock() {
    if let Err(e) = listen.connect() {
      eprintln!("Failed to connect: {e}");
      return;
    }
  }

  DebugService::new(Arc::clone(&listener));
  WorkspaceService::new(Arc::clone(&listener));

  // This increases the reference count.
  // This does not clone the listener itself. It only clones the Arc pointer,
  let handle = HyprlandIpc::start(Arc::clone(&listener));
  println!("Main thread continues...");

  handle.join().expect("Listener thread paicked");
}
