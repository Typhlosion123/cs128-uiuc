#include "db_table.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

void DbTable::FreeRow(void** row, unsigned int cols) { //freeeing the rows is prolly better optimized if i do it here cause we lowk be doing it alot
    for (unsigned int i = 0; i < cols; ++i) {
        if(col_descs_[i].second == DataType::kString) {
            delete static_cast<std::string*>(row[i]);
        } else if (col_descs_[i].second == DataType::kInt) {
            delete static_cast<int*>(row[i]);
        } else if (col_descs_[i].second == DataType::kDouble) {
            delete static_cast<double*>(row[i]);
        }
    }
}

void DbTable::CopyRow(void** destination, void** source, unsigned int cols) { //copy constrution for rows
    for (unsigned int i = 0; i < cols; ++i) {
        if (col_descs_[i].second == DataType::kDouble) {
            destination[i] = new double(*static_cast<double*>(source[i]));
        } else if (col_descs_[i].second == DataType::kInt) {
            destination[i] = new int(*static_cast<int*>(source[i]));
        } else if (col_descs_[i].second == DataType::kString) {
            destination[i] = new std::string(*static_cast<std::string*>(source[i]));
        }
    }
}

DbTable::DbTable(const DbTable& rhs) {
    next_unique_id_ = rhs.next_unique_id_; //copy everything over
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;
    for (const auto& [id, row] : rhs.rows_) { //legit copy everyting over, this tie its the rows
        void** new_row = new void*[row_col_capacity_];
        CopyRow(new_row, row, col_descs_.size());
        rows_[id] = new_row;
    }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
    if (this == &rhs) { //check for self referal
        return *this;
    }
    for (auto& [id, row] : rows_) { //free the rows!!
        FreeRow(row, col_descs_.size());
        delete[] row;
    }
    rows_.clear();
    next_unique_id_ = rhs.next_unique_id_; //copy all
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;
    for (const auto& [id, row] : rhs.rows_) {
        void** new_row = new void*[row_col_capacity_];
        CopyRow(new_row, row, col_descs_.size());
        rows_[id] = new_row;
    }
    return *this;
}

DbTable::~DbTable() {
    for (auto& [id, row] : rows_) {
        FreeRow(row, col_descs_.size());
        delete[] row;
    }
}

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
    if (col_descs_.size() == row_col_capacity_) { //change size
        unsigned int new_capacity = row_col_capacity_ * 2;
        for (auto& [id, row] : rows_) {
            void** new_row = new void*[new_capacity];
            for (unsigned int i = 0; i < col_descs_.size(); ++i) {
                new_row[i] = row[i];
            }
            delete[] row;
            row = new_row;
        }
        row_col_capacity_ = new_capacity;
    }
    col_descs_.push_back(col_desc); //add the new column
    for (auto& [id, row] : rows_) { //add the new column based upon data type
        if (col_desc.second == DataType::kString) {
            row[col_descs_.size() - 1] = new std::string();
        } else if (col_desc.second == DataType::kInt) {
            row[col_descs_.size() - 1] = new int(0);
        } else if (col_desc.second == DataType::kDouble) {
            row[col_descs_.size() - 1] = new double(0.0);
        }
    }
}

void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
    if (col_idx >= col_descs_.size()) { //check range
        throw std::invalid_argument("Column index out of range (DeleteColumnVByIDX)");
    }
    if (col_descs_.size() == 1 && !rows_.empty()) { //check empty
        throw std::invalid_argument("cannot delete the last row standing (delete colum by iDX)");
    }
    DataType type = col_descs_[col_idx].second; //find datatype
    for (auto& [id, row] : rows_) { //delete absed upon row
        if (type == DataType::kString) {
            delete static_cast<std::string*>(row[col_idx]);
        } else if (type == DataType::kInt) {
            delete static_cast<int*>(row[col_idx]);
        } else if (type == DataType::kDouble) {
            delete static_cast<double*>(row[col_idx]);
        }
        for (unsigned int i = col_idx; i < col_descs_.size() - 1; ++i) {
            row[i] = row[i + 1]; //push everything back in the row
        }
    }
    col_descs_.erase(col_descs_.begin() + col_idx); //erase the column descriptoin
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
    if (col_data.size() != col_descs_.size()) { //checl column data
        throw std::invalid_argument("Ts colmns dont match (add row)");
    }
    void** new_row = new void*[row_col_capacity_]; //new row!!
    unsigned int i = 0; //iterator to go through each value in the column
    for (const auto& data : col_data) { //each value in the column
        if (col_descs_[i].second == DataType::kString) {
            new_row[i] = new std::string(data);
        } else if (col_descs_[i].second == DataType::kInt) {
            new_row[i] = new int(std::stoi(data));
        } else if (col_descs_[i].second == DataType::kDouble) {
            new_row[i] = new double(std::stod(data));
        }
        ++i;
    }
    rows_[next_unique_id_] = new_row;
    next_unique_id_++;
    
}

void DbTable::DeleteRowById(unsigned int id) {
    auto iterator = rows_.find(id); //find row
    if (iterator == rows_.end()) { //check if existence
        throw std::invalid_argument("Row id cannot be found (deleteowbyid)");
    }
    void** row = iterator->second; //find row
    FreeRow(row, col_descs_.size());
    delete[] row; //finish him
    rows_.erase(iterator);
}



std::ostream& operator<<(std::ostream& os, const DbTable& table) {
    for (unsigned int i = 0; i < table.col_descs_.size(); ++i) {
        os << table.col_descs_[i].first << "(";
        if (table.col_descs_[i].second == DataType::kDouble) {
            os << "double";
        } else if (table.col_descs_[i].second == DataType::kInt) {
            os << "int";
        } else if (table.col_descs_[i].second == DataType::kString) {
            os << "std::string";
        }
        os << ")";
        if (i < table.col_descs_.size() - 1) {
            os << ", ";
        } else {
            os << "";
        }
    }
    os << "\n";
    for (const auto& [id, row] : table.rows_) {
        for (unsigned int i = 0; i < table.col_descs_.size(); ++i) {
            if (table.col_descs_[i].second == DataType::kDouble) {
                os << *(static_cast<double*>(row[i]));
            } else if (table.col_descs_[i].second == DataType::kInt) {
                os << *(static_cast<int*>(row[i]));;
            } else if (table.col_descs_[i].second == DataType::kString) {
                os << *(static_cast<std::string*>(row[i]));;
            }
            if (i < table.col_descs_.size() - 1) {
                os << ", ";
            }
        }
        os << "\n";
    }
    return os;
}
