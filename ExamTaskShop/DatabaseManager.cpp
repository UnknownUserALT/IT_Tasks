
#include "DatabaseConnection.hpp"

class DatabaseManager {
private:
    static std::shared_ptr<DatabaseConnection> instance;
    
public:
    static std::shared_ptr<DatabaseConnection> getInstance() {
        if (!instance) {
            instance = std::make_shared<DatabaseConnection>(
                "host=localhost port=5432 dbname=online_store user=postgres password=288288");
        }
        return instance;
    }
    
    static void initialize(const std::string& connectionString) {
        if (!instance) {
            instance = std::make_shared<DatabaseConnection>(connectionString);
        }
    }
};