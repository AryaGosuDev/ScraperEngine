#include <string>


class ThreadHandler {
 public:
  virtual std::string Handle(std::string request) = 0;
};

class AbstractThreadHandler : public ThreadHandler {
 private:
 public:
  AbstractHandler() {}
  virtual std::string Handle(std::string request) override {
    return {};
  }
};

class ScrapeThread : public AbstractThreadHandler {
};

class VerificationThread : public AbstractThreadHandler {
};

