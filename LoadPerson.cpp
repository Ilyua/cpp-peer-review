vector<Person> loadPersons(string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
                           DBLogLevel db_log_level, int min_age, int max_age, string_view name_filter) {
    DBHandler db = establishDatabaseConnection(db_name, db_connection_timeout, db_allow_exceptions, db_log_level);
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    string query_str = constructQuery(min_age, max_age, name_filter, db);
    DBQuery query(query_str);

    return fetchPersonsFromDB(db, query);
}

DBHandler establishDatabaseConnection(string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
                                      DBLogLevel db_log_level) {
    DBConnector connector(db_allow_exceptions, db_log_level);
    if (db_name.starts_with("tmp."s)) {
        return connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        return connector.Connect(db_name, db_connection_timeout);
    }
}

string constructQuery(int min_age, int max_age, string_view name_filter, const DBHandler& db) {
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    return query_str.str();
}

vector<Person> fetchPersonsFromDB(DBHandler& db, const DBQuery& query) {
    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.emplace_back(move(name), age);
    }
    return persons;
}
