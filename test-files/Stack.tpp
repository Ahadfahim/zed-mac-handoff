// Stack.tpp
template <typename T>
void Stack<T>::push(const T& value) { 
  data_[size_] = value;
  size_++;
}
