namespace mycpp {

template <typename T> class my_shared_ptr {
public:
  // default constructor
  my_shared_ptr() : ptr_(nullptr), count_ref_(new unsigned int(0)){};
  my_shared_ptr(T *p) : ptr_(p), count_ref_(new unsigned int(1)) {}
  // copy semantics
  my_shared_ptr(const my_shared_ptr &p) {
    this->ptr_ = p.ptr_; // friend
    this->count_ref_ = p.count_ref_;
    if (p.ptr_ != nullptr) {
      (*count_ref_)++;
    }
  }
  my_shared_ptr &operator=(const my_shared_ptr &p) {
    // clean up self existed data
    __clean_up__();
    this->ptr_ = p.ptr_;
    this->count_ref_ = p.count_ref_;
    if (p.ptr_ != nullptr) {
      (*count_ref_)++;
    }
  }

  // move semantics
  my_shared_ptr(my_shared_ptr &&p) {
    this->ptr_ = p.ptr_;
    this->count_ref_ = p.count_ref_;
    p.ptr_ = nullptr;
    p.count_ref_ = nullptr;
  }

  my_shared_ptr &operator=(my_shared_ptr &&p) {
    // clean up self existed data
    __clean_up__();
    this->ptr_ = p.ptr_;
    this->count_ref_ = p.count_ref_;

    p.ptr_ = nullptr;
    p.count_ref_ = nullptr;
  }

  ~my_shared_ptr() { __clean_up__(); }

public:
  T *operator->() const { return this->ptr_; }
  T &operator*() const { return *this->ptr_; }

private:
  void __clean_up__() {
    if (this->ptr_ != nullptr) {
      (*this->count_ref_)--;
      if (*this->count_ref_ == 0) {
        delete this->ptr_;
        delete this->count_ref_;
      }
    }
  }

private:
  unsigned int *count_ref_;
  T *ptr_;
};

}; // namespace mycpp
