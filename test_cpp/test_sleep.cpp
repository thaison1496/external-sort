 #include <unistd.h>
  #include <iostream> 

  int main() {
    std::cout << "hello" << std::endl;

    int i = 0;
    while(i < 10000) {
      ++i;
      sleep(1);
      std::cout << "i = " << i << std::endl;                                                                                                                                        
    }
    std::cout << "bye" << std::endl;
    return 0;
  }