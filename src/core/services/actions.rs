use std::borrow::Cow;
use std::collections::HashMap;
use std::rc::Rc;

use crate::core::actions::Action;

pub struct ActionsManager {
    actions: HashMap<Cow<'static, str>, Rc<Action>>,
}

impl ActionsManager {
    pub fn new() -> Self {
        ActionsManager {
            actions: Default::default(),
        }
    }

    pub fn actions(&self) -> Vec<Rc<Action>> {
        self.actions.values().cloned().collect()
    }

    pub fn action_ids(&self) -> Vec<Cow<'static, str>> {
        self.actions.keys().cloned().collect()
    }

    pub fn get_action(&self, id: &str) -> Option<Rc<Action>> {
        self.actions.get(id).cloned()
    }

    pub fn register(&mut self, action: Rc<Action>) {
        self.actions.insert(action.cow_id(), action);
    }
}
