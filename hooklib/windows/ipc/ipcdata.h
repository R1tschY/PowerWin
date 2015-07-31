#ifndef HOOKLIB_WINDOWS_IPC_IPCDATA_H_
#define HOOKLIB_WINDOWS_IPC_IPCDATA_H_

namespace Windows {

class IPCData {
public:
  template<typename T>
  const T& get() const
  {
    if (sizeof(T) != data_.size())
    {
      throw std::runtime_error("wrong data size");
    }
    return *reinterpret_cast<const T*>(data_.data());
  }

  template<typename T>
  void set(const T& value)
  {
    auto* ptr = reinterpret_cast<const char*>(value);
    data_.assign(value, value + sizeof(T));
  }

private:
  std::vector<char> data_;
};

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_IPC_IPCDATA_H_ */
