#include "db.hpp"
#include <stdexcept>

void Database::CreateTable(const std::string& table_name) { 
    if(tables_.find(table_name) != tables_.end()) { //check existnce
        throw std::runtime_error("Table already exists (CreateTable)");
    }
    tables_[table_name] = new DbTable();
}

void Database::DropTable(const std::string& table_name) {
    auto iterator = tables_.find(table_name);
    if (iterator == tables_.end()) { //check existence
        throw std::runtime_error("Table not found and cannot be dropped");
    }
    delete iterator->second;
    tables_.erase(iterator);
}

DbTable& Database::GetTable(const std::string& table_name) {
    auto iterator = tables_.find(table_name);
    if (iterator == tables_.end()) { //check exisntence
        throw std::runtime_error("Cannot locate table in GetTable");
    }
    return *(iterator->second); //I think this is right....
}

Database::Database(const Database& rhs) {
    for (const auto& [name, table] : rhs.tables_) { ///just populate properly
        tables_[name] = new DbTable(*table);
    }
}

Database& Database::operator=(const Database& rhs) {
    if (this == &rhs) { //self check
        return *this;
    }
    for (auto& [name, table] : tables_) { //free all
        delete table;
    }
    tables_.clear();
    for (const auto& [name, table] : rhs.tables_) { //opopylate
        tables_[name] = new DbTable(*table);
    }
    return *this;
}

Database::~Database() {
    for (auto& [name, table] : tables_) {
        delete table;
    }
    tables_.clear();
}


std::ostream& operator<<(std::ostream& os, const Database& db) {
    for (const auto& [name, table] : db.tables_) {
        os << "Table: " << name << "\n";
        os << *table;
    }
    return os;
}
