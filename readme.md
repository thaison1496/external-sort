# External sort
Sort lines of a very large text file lexicographically, each line is a unicode string seperated with \n

The sort algorithm assume input file in utf-8 encoding, and character is ordered by their unicode's code point
&nbsp;
##### Compile and run:

```sh
g++ main.cpp -std=c++11 -pthread && ./a.out [input_file] [output_file] [available RAM (in MB)]
```

##### Example:
```sh
g++ main.cpp -std=c++11 -pthread && ./a.out test/500mb_ascii test/500mb_ascii.out 10
```

