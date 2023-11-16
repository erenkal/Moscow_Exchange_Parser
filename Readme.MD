
# PCAP and Simba Parser

This project provides a custom-built parser for `.pcap` files (commonly used for capturing network traffic) and the Simba binary protocol format (used by the Moscow Exchange). It uses C++23 features and is built using CMake.
Especially Simba part is written with great care and attention to details.
## Project Highlights:
- **Parsing Types**: The parser can parse both `.pcap` files and Simba binary protocol files.
- **Performance Improvement**: Reduced the Simba file parsing time from an initial 73 seconds (first version of code) to a final 0.7-1.5 seconds with average packet read time 250-500 nanoseconds on my macbook air depends on the load. 
(There are improvements that can be made to further reduce the time)
- **Code Readability**: The code is well documented and easy to understand.

## Parser's Architecture:
The parser is implemented as a header-only library (Parser.h). All functions are inlined reinterpret_cast wrappers and use templates for maximum performance.
We could use reinterpret_cast with whole structs to parse but it makes debugging harder when writing everything from scrath. And we must make pragma pack(1) to 
avoid padding but Padding is good for performance. So we have used reinterpret_cast with individual members of struct and also we have used templates for maximum performance.
Also our methods supports Little endian to Big endian conversions vice versa. (also implemented in Parser.h but not used in this project after testing)

## SimbaWriter's Architecture:
When I first tried to create strings for logging I can only get 20~ seconds for 2.5 million packets. So I have started creating them in an async thread (it can also be done in a thread pool)
and I have used a non-blocking logger (nanolog) to write the output file. It has improved the performance a lot. Now I can get 0.7-1.5 seconds for decoding 2.5 million packets. (depends on the load of my machine)
It sounds trivial but in a real world scenario logging should be done in a separate thread like this.
We also used likely and unlikely(c++23) attributes to help the compiler to optimize the code with branch prediction.

## Building:
Ensure you have CMake and an appropriate build system (like Ninja or Make) installed on your system.
(I have used Ninja for this project but instructions for Make provided below)
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```


## Usage:

### PCAP Parsing:

This will parse the pcap file and convert the data into an intermediary text format.

```bash
./parser p <path_to_pcap_file> <output_txt_file>
```

**Example:**

```bash
./parser p 2023-04-25.1849-1906.pcap out.txt
```

### Simba Parsing:

This takes the intermediary text file from the previous step and decodes the Simba binary protocol. The results will be saved to the specified output file.

```bash
./parser s <path_to_intermediary_txt> <output_file>
```

**Example:**

```bash
./parser s out.txt outsimba3
```


## Supported Simba Messages:

This parser focuses on three main message types from the Simba protocol:
1. OrderUpdate
2. OrderExecution
3. OrderBookSnapshot

For a detailed explanation of these message types, refer to the [official Simba format specification](http://ftp.moex.ru/pub/SIMBA/Spectra/prod/doc/spectra_simba_en.pdf).

## Example Output:
```
---- SNAPSHOT PACKET ----
Security ID: 3495103
Last Message Sequence Number Processed: 22037630
Report Sequence: 3
Exchange Trading Session ID: 6785
---- Market Data Entries ----
    MDEntry
    Entry Id : 1147124
    Transaction Time: 1682409469887594353
    Price: 5.7430
    Entry Size: 10
    Trade Id: 14125
    Flags : * 0x1 Day
    * 0x1000 - End of transaction bit
    Entry Type: ASK
---- End of Snapshot Packet ----
```
## Used C++ Concepts:
- **Templates**
- **Single Writer Single Reader Lock Free Queue**: Written with atomic structures. Used for maximum performance.
- **Non Blocking Logger**
- **Attributes**: Used for helping the compiler to optimize the code.
- **string_view**: std::span can also be used but I am more familiar with string_view.
- **atomic_structures**
- **macros for network byte order conversions**: (not used in this project after testing)
- and many more...
## Further Improvements:
I want to implement the following improvements in the future:
- **Better Error Handling**: Currently, the parser does not handle errors very well. It simply exits the program when an error occurs. I want to implement a better error handling system that will allow the parser to continue parsing even if an error occurs.
- **Better Performance**: I want to implement a better performance system that will allow the parser to parse the files faster.
- **Thread Pooling**: Since we are parsing a big file and we have to parse it in chunks, we can use thread pooling to parse the chunks in parallel. This could improve the performance of the parser. But in the real market data parsing I think it won't make a big difference (especially when data is sent from different parts) but it can be tested.
- **Sequential Writer**: We can also use a sequential writer written with atomic structures to write the output file. But our nanolog implementation is better because it's nonblocking.
- **More Message Types**: Other message types can be implemented in the future.

---
