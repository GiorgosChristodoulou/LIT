# LIT: Lightning-fast In-memory Temporal Indexing

Source code from the publication at SIGMOD '24:
- <i>LIT: Lightning-fast In-memory Temporal Indexing</i>

## Dependencies
- g++/gcc
- Boost Library 


## Data

Directory  ```streams``` includes  a BOOKS stream used in the experiments which contains 10K queries and 4625204 updates 
- BOOKS.mix



## Compile
Compile using ```make all``` or ```make <option>``` where `<option>` can be one of the following:
   - pureLIT 
   - teHINT
   - aLIT


## Shared parameters among all methods
| Parameter | Description | Comment |
| ------ | ------ | ------ |
| -? or -h | display help message | |
| -r | set the number of runs per query; by default 1 |  |


## Workloads
The code supports two types of workload:
- Counting the qualifying records, or
- XOR'ing between their ids

You can switch between the two by appropriately setting the `WORKLOAD_COUNT` flag in def_global.h; remember to use `make clean` after resetting the flag. 


## Indexing and query processing methods

### Time-Evolving HINT:

#### Source code files
- main_teHINT.cpp
- containers/relation.h
- containers/relation.cpp
- indices/hint_m_dynamic_naive.cpp
- indices/hint_m.h

#### Execution
| Extra parameter | Description | Comment |
| ------ | ------ | ------ |
| -e | set the leaf partition extent | 86400 for the example stream in the experiments |

- ##### Examples

    ```sh
    $ ./query_teHINT.exec -e 86400 streams/BOOKS.mix
    ```

### pureLIT: 

#### Source code files
- main_pureLIT.cpp
- containers/relation.h
- containers/relation.cpp
- containers/buffer.h
- containers/buffer.cpp
- indices/hierarchicalindex.h
- indices/hierarchicalindex.cpp
- indices/live_index.h
- indices/live_index.cpp
- indices/hint_m.h
- indices/hint_m_dynamic.cpp

#### Execution
| Extra parameter | Description | Comment |
| ------ | ------ | ------ |
| -e | set the leaf partition extent |  |
| -b | set the type of data structure for the LIVE INDEX |  |
| -c | set the capacity constraint number for the LIVE INDEX |  |
| -d | set the duration constraint number for the LIVE INDEX |  |

- ##### Examples    

    ```sh
    $ ./query_pureLIT.exec -e 86400 -b ENHANCEDHASHMAP -c 10000 -r 10 streams/BOOKS.mix
    ```


### aLIT: 

#### Source code files
- main_aLIT.cpp
- containers/relation.h
- containers/relation.cpp
- containers/buffer.h
- containers/buffer.cpp
- indices/hierarchicalindex.h
- indices/hierarchicalindex.cpp
- indices/live_index.h
- indices/live_index.cpp
- indices/hint_m.h
- indices/hint_m_dynamic.cpp

#### Execution
| Extra parameter | Description | Comment |
| ------ | ------ | ------ |
| -e | set the leaf partition extent |  |
| -b | set the type of data structure for the LIVE INDEX |  |
| -c | set the capacity constraint number for the LIVE INDEX |  |
| -d | set the duration constraint number for the LIVE INDEX |  |
| -n | set the number of LIT indices for non-temporal attribute indexing |  |

- ##### Examples

    ###### 
    ```sh
    $  ./query_aLIT.exec -e 86400 -b ENHANCEDHASHMAP -c 10000 -n 8  -r 10 streams/BOOKS.mix
    ```


