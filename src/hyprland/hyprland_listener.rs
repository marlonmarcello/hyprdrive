use std::{
  collections::HashMap,
  env,
  io::{self, BufRead, BufReader},
  os::unix::net::UnixStream,
  sync::{Arc, Mutex},
  thread,
};

use super::hyprland_event::{EventCallback, HyprlandEvent, SubscriptionId};

#[derive(Default)]
pub struct HyprlandListener {
  stream: Option<UnixStream>,
  subscribers: HashMap<SubscriptionId, EventCallback>,
  next_id: SubscriptionId,
}

impl HyprlandListener {
  pub fn connect(&mut self) -> io::Result<()> {
    let xdg_dir = env::var("XDG_RUNTIME_DIR").map_err(|e| io::Error::new(io::ErrorKind::NotFound, e))?;
    let signature = env::var("HYPRLAND_INSTANCE_SIGNATURE").map_err(|e| io::Error::new(io::ErrorKind::NotFound, e))?;
    let socket_path = format!("{xdg_dir}/hypr/{signature}/.socket2.sock");

    let stream = UnixStream::connect(socket_path)?;
    println!("[HyprlandListener] Connected");
    self.stream = Some(stream);
    Ok(())
  }

  pub fn subscribe(&mut self, callback: EventCallback) -> SubscriptionId {
    let id = self.next_id;
    self.next_id += 1;
    self.subscribers.insert(id, callback);
    id
  }

  pub fn unsubscribe(&mut self, id: SubscriptionId) {
    self.subscribers.remove(&id);
  }

  pub fn start(listener: Arc<Mutex<Self>>) -> thread::JoinHandle<()> {
    thread::spawn(move || {
      let stream = match listener.lock() {
        Ok(mut listen) => listen.stream.take(),
        Err(_) => None,
      };

      if let Some(stream) = stream {
        let mut reader = BufReader::new(stream);
        let mut line_buffer = String::new();

        println!("[HyprlandListener] Starting event loop...");
        while let Ok(bytes_read) = reader.read_line(&mut line_buffer) {
          if bytes_read == 0 {
            eprintln!("[HyprlandListener] Connection closed or error.");
            break;
          }

          let message = line_buffer.trim();
          if message.is_empty() {
            continue;
          }

          let mut event = HyprlandEvent::default();
          if let Some((name, data)) = message.split_once(">>") {
            event.name = name.to_string();
            event.data = data.to_string();
          } else {
            event.data = message.to_string();
          }

          if let Ok(listen) = listener.lock() {
            for callback in listen.subscribers.values() {
              callback(event.clone());
            }
          }
          line_buffer.clear();
        }
      }
      eprintln!("[HyprlandListener] Connection closed or error.");
    })
  }
}
