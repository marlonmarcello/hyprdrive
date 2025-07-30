#[derive(Debug, Clone, Default)]
pub struct HyprlandEvent {
  pub name: String,
  pub data: String,
}

pub type EventCallback = Box<dyn Fn(HyprlandEvent) + Send>;
pub type SubscriptionId = usize;
