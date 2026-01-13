//
// Created by Worker on 13/1/2026 AD.
//

#ifndef EXAMTASKSHOP_DATABASE_SETUP_HPP
#define EXAMTASKSHOP_DATABASE_SETUP_HPP
#include "../DatabaseConnection.hpp"

bool createTables(DatabaseConnection& db);
bool createIndexes(DatabaseConnection& db);
void verifyDatabaseSetup(DatabaseConnection& db);
#endif //EXAMTASKSHOP_DATABASE_SETUP_HPP