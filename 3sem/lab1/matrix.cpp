#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "matrix.h"

namespace Matrix_funcs {
    void input(Matrix &matrix){
        int num = 0;
        bool firstNonZero = true;

        try {
            std::cout << "Enter number of lines in matrix:" << std::endl;
            matrix.linesNum = getNum<int>();
            std::cout << "Enter number of rows in matrix:" << std::endl;
            matrix.rowsNum = getNum<int>();

            matrix.values.resize(2);
            matrix.rowsIds.resize(2);
            matrix.firstNonZeroPtrs.resize(2);
            matrix.zeroLines.resize(2);

            for (int i = 0; i < matrix.linesNum; i++){
                firstNonZero = true;
                std::cout << "Enter line number " << i + 1 << ":" << std::endl;
                for (int j = 0; j < matrix.rowsNum; j++){
                    num = getNum<int>();
                    if (num != 0){
                    	if (matrix.values.size() == matrix.nonZeroNum + 1){
                    		matrix.values.resize(matrix.values.size() * 2);
                    		matrix.rowsIds.resize(matrix.rowsIds.size() * 2);
                    	}
                        matrix.values[matrix.nonZeroNum] = num;
                        matrix.rowsIds[matrix.nonZeroNum] = j;
                        if (firstNonZero){
	                        if (matrix.firstNonZeroPtrs.size() == matrix.ptrNum + 1){
	                        	matrix.firstNonZeroPtrs.resize(matrix.firstNonZeroPtrs.size() * 2);
	                        }
                            matrix.firstNonZeroPtrs[matrix.ptrNum++] = matrix.nonZeroNum;
                            firstNonZero = false;
                        }
                        matrix.nonZeroNum++;
                    }
                }
                if (firstNonZero){
                	if (matrix.zeroLines.size() == matrix.zeroLinesNum + 1){
                		matrix.zeroLines.resize(matrix.zeroLines.size() * 2);
                	}
                    matrix.zeroLines[matrix.zeroLinesNum++] = i;
                }
            }
            matrix.firstNonZeroPtrs[matrix.ptrNum++] = matrix.nonZeroNum;
            matrix.values.resize(matrix.nonZeroNum);
            matrix.rowsIds.resize(matrix.nonZeroNum);
            matrix.firstNonZeroPtrs.resize(matrix.ptrNum);
            matrix.zeroLines.resize(matrix.zeroLinesNum);
        }
        catch (...) { // в случае любого исключения
            erase(matrix); // очищаем уже выделенную память
            throw; // перекидываем то же исключение дальше
        }
    }

    void output(const Matrix &matrix){
        if (matrix.values.empty() && matrix.zeroLines.empty()){
            throw std::runtime_error("Matrix is not initialized! Please input a matrix first.");
        }
        int lines = matrix.linesNum, rows = matrix.rowsNum;
        int zeroLinesNum = matrix.zeroLinesNum;
        std::vector<int> values = matrix.values;
        std::vector<int> rowsIds = matrix.rowsIds;
        std::vector<int> firstNonZeroPtrs = matrix.firstNonZeroPtrs;
        std::vector<int> zeroLines = matrix.zeroLines;
        int countZero = 0;

        std::cout << std::endl;

        for (int i = 0; i < lines; i++){
            if (countZero < zeroLinesNum && i == zeroLines[countZero]){
                for (int j = 0; j < rows; j++){
                    std::cout << "0 ";
                }
                countZero++;
            } else {
                int start = firstNonZeroPtrs[i - countZero];
                int end = firstNonZeroPtrs[i - countZero + 1];
                int currRowId = 0;

                for (int j = start; j < end; j++){
                    while (currRowId < rowsIds[j]){
                        std::cout << "0 ";
                        currRowId++;
                    }
                    std::cout << values[j] << " ";
                    currRowId++;
                }

                while (currRowId < rows){
                    std::cout << "0 ";
                    currRowId++;
                }
            }
            std::cout << std::endl;
        }
    }

    void erase(Matrix &matrix){
        matrix.values.clear();
        matrix.rowsIds.clear();
        matrix.firstNonZeroPtrs.clear();
        matrix.zeroLines.clear();
        matrix.linesNum = 0;
        matrix.rowsNum = 0;
        matrix.nonZeroNum = 0;
        matrix.zeroLinesNum = 0;
        matrix.ptrNum = 0;
    }

    std::vector<int> process(const Matrix &matrix){
        if (matrix.values.empty() && matrix.zeroLines.empty()){
            throw std::runtime_error("Matrix is not initialized! Please input a matrix first.");
        }
        int lines = matrix.linesNum;
        int zeroLinesNum = matrix.zeroLinesNum;
        std::vector<int> values = matrix.values;
        std::vector<int> firstNonZeroPtrs = matrix.firstNonZeroPtrs;
        std::vector<int> zeroLines = matrix.zeroLines;
        int ptrNum = matrix.ptrNum, nonZeroNum = matrix.nonZeroNum;
        int countZero = 0, start = 0, end = 0;
        std::unordered_set<int> set;
        bool flag = false;
        try {
        	std::vector<int> vector(lines);
	        for (int i = 0; i < lines - 1; i++){
	            flag = false;
	            if (countZero < zeroLinesNum && i == zeroLines[countZero]){ // [i] = 0, [i+1] = {0,1}
	                vector[i] = 0;
	                countZero++;
	            } else {
	                if (zeroLinesNum){ // [i] = 1, [i+1] = 0
	                    for (int el : zeroLines){
	                        if (el == i + 1){
	                            flag = true;
	                            break;
	                        }
	                    }
	                    if (flag){
	                        start = firstNonZeroPtrs[i - countZero];
	                        end = firstNonZeroPtrs[i - countZero + 1];
	                        for (int j = start; j < end; j++){
	                            if (values[j] != 0){
	                                vector[i]++;
	                            }
	                        }
	                        continue;
	                    }
	                }
	                // [i] = 1, [i+1] = 1
	                start = firstNonZeroPtrs[i + 1 - countZero];
	                end = firstNonZeroPtrs[i - countZero + 2];

	                for (int j = start; j < end; j++){
	                    set.insert(values[j]);
	                }

	                start = firstNonZeroPtrs[i - countZero];
	                firstNonZeroPtrs[i - countZero + 1];

	                for (int j = start; j < end; j++){
	                    if (set.find(values[j]) == set.end()){
	                        vector[i]++;
	                    }
	                }
	                set.clear();
	            }
	        }

	        if (!zeroLinesNum || (lines - 1 != zeroLines[zeroLinesNum - 1])){
	            for (int i = firstNonZeroPtrs[ptrNum - 2]; i < nonZeroNum; i++){
	                if (values[i] != 0){
	                    vector[lines - 1]++;
	                }
	            }
	        }

	        std::cout << "Here is non-sorted array:" << std::endl;
	        for (int elem: vector){
	            std::cout << elem << ' ';
	        }
	        std::cout << std::endl;

	        return vector;
        }

        catch(...){
                	throw std::runtime_error("No memory for vector!");
                }
    }

    void sort_array(std::vector<int> &array){
        if (array.empty()){
            throw std::runtime_error("Array is empty!");
        }
        std::cout << "Here is sorted array";

        if (array.size() > 1){
            if (array[0] < array[1]){
                sort(array.begin(), array.end());
                std::cout << " (straight order)";
            } else if (array[0] > array[1]){
                sort(array.begin(), array.end(), std::greater<>());
                std::cout << " (reversed order)";
            } else {
                std::cout << " (same order)";
            }
        }
        std::cout << ":" << std::endl;

        for (int elem: array){
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}
