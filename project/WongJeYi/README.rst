=================
ColTable
=================
Project can be found here
`WongJeYi <https://github.com/WongJeYi>`__ for
   `ColTable <WongJeYi/ColTable/README.rst>`__:
   https://github.com/WongJeYi/ColTable
Basic Information
=================


The ColTable is a way to store tables of data in a column major format for better query performance.
ColTable includes add, replace, remove, index, concatenation, and length.


Problem to Solve
================
Table consists of columns and rows, where columns are fixed number of classes, and rows can be added.
Storing a Table in memory by rows is inefficient for accessing memories. 
Rows vs Columns:

+-----+-----+--------+--------+
| Dog | Age | Weight | Gender |
+=====+=====+========+========+
| A   | 1   | 10     | Male   |
+-----+-----+--------+--------+
| B   | 2   | 21     | Female |
+-----+-----+--------+--------+

Comparison for Advantages:
Rows	: New dogs can be added, modified, or deleted quickly.
Columns	: Data analysis by Age/ Weight/ Gender of dogs can be analyzed easily

Prospective Users
=================

- Data scientist: Computing with big set of numbers 
- Performance engineer: Optimize Python framework with high numerical performance without language dependency

System Architecture
===================

Step 1: The ColTable Python constructor takes an input of the array, and the datatypes that it contains:
The assumption is the array must be the same datatype as requested in the constructor.

.. code-block:: python

   ColTable(Array<int | double | float | String | List | Dict | Struct> var, format format)

.. code-block:: python

Step 2: The Pybind wrapper will pass the data to the C++ library for computation.

Step 3: Initialize of ColTable, ColTableField, ColTableData. 
ColTableField will store the parent and child information. 
ColTableData will store the data information.
Buffer class will help to serialize the data into columnar array buffer.

For the ColTable, there are various possibility of data types. Therefore, we need a few abstract classes as below.


.. code-block:: cpp

   class ColTable {
       public:
           std::shared_ptr<ColTableField> field;

           std::shared_ptr<ColTableData> data;

   };


   class ColTableField{
      public:
         std::string key;

         format format;

         std::vector<std::shared_ptr<ColTableField>> children;

         virtual int64 nChildren;

   }


   class ColTableData{
      public:
         std::vector<std::shared_ptr<ColTableData>> children; 

         int64_t length;

         int64_t offsetBuffer;

         int64_t nullCount;

         int64_t validityBitmapsBuffer;

         int64_t nChildren;

         std::shared_ptr<Buffer> valueBuffer;

   }

   Buffer buffer(){
   //Handle buffer storage in columnar array format
   }

   enum class format {

      INT32,
      DOUBLE,
      FLOAT,
      STRING,
      LIST,
      STRUCT,
      DICT,
      UNKNOWN

   }

.. code-block:: cpp

**key:** var name

**format:** describe the datatypes

**children:**	Optional If var contains an Array<Object>/List/Dict in itself--> children will contain a new ColTableData

**valueBuffer:** This contains the actual data

**offsetBuffer:**	Optional This helps to define the start and end position in value buffer	

**nullCount:** This buffer is to calculate total null. For every null, increase count by 1

**validityBitmapsBuffer:** This is a 0 or 1 bit of the length of actual data. 0 indicates null, 1 indicates non null

**nChildren:** Optional Count number of children

API Description
===============

The python calling constructor will import the array and array will be converted into colTable.

.. code-block:: python

   import colTable as ct

   age = ct.array([1,2,3], format="INT32")

   name = ct.array(["A","B","C"], format="STRING")

   scores = ct.array([[1,2],[3,4]], format="LIST")

.. code-block:: python

Engineering Infrastructure
==========================

The project will be built using Cmake v3.11 to make the C++ code compilation, 
Then use pybind11_add_module in Cmake, run Cmake to build the pybind module.
Version control is managed through Git, where all code updates are submitted via Pull Requests accompanied by comments detailing the specific changes. 
Software correctness is verified by the google test framework using a round-trip test.
This is tested by converting an array into ColTable and converting back to Array, the data type should remain the same and the data is the same.
Continuous Integration must pass the Ubuntu test. 
These must be passed before merging.

Schedule
========

Week 1 (03/09):
Definition for c++ ColTable data structure
Start presentation slide preparation and update according to weekly progress

Week 2 (03/16):
Definition for c++ table buffer operation functions
Create test bench for c++ round trip functions

Week 3 (03/23):
Create indexing, slicing, getting, replace function in c++

Week 4 (03/30):
command testbench for python call
Python wrapper classes constructor

Week 5 (04/06):
Troubleshooting week or completing delayed progress

Week 6 (04/13):
Optimization for memory and adding more complicated data types like Dictionary if possible

Week 7 (04/20):
Adding option for adaptive row or Columnary array function if possible
.. 
   State 1: Initial: Store array in row array.
   State 2: Tracking: Once an array exceeds a size threshold (e.g 50,000 elements), the system begins counting read accesses.
   State 3: Transformation: If read counts exceed a second threshold (default 25,000), the system triggers the actual memory layout transformation.
   State 4: Restoration: If an unsupported operation occurs (e.g., adding an incompatible object shape or modifying a property), the array is automatically reverted to its original row-oriented format.

Week 8 (04/27):
Completing the final pieces/ refactoring

References
==========

• https://arrow.apache.org/docs/format/CDataInterface.html
• https://dl.acm.org/doi/10.1145/3546918.3546919

Project ColTable list by WongJeYi
