#ifndef MESSAGESINK_H
#define MESSAGESINK_H

#include "../control.h"

#include <functional>

namespace Windows {

class MessageSink : protected Control
{
public:
  typedef std::function<LRESULT(UINT, WPARAM, LPARAM)> MessageFunc;

  explicit MessageSink(MessageFunc wndproc = nullptr);

  void create();

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
  MessageFunc wndproc_;
};

} // namespace Windows

#endif // MESSAGESINK_H
