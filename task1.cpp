#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>

using namespace std;

class EBook {
public:
    // Обновление последней страницы пользователя
    void UpdateUserLastPage(int user, int page) {
        DecreaseCounterPrevUserPage(user);
        UpdateUserPage(user, page);
        IncreaseCounterNewUserPage(page);
    }

    // Вычисление доли страниц, которые меньше текущей страницы пользователя
    double CalculateFractionOfLessPages(int user) {
        if (userToPagesCount.empty()) {
            return 0.0;
        } else if (IsUserAlone(user)) {
            return 1.0;
        } else if (userToPagesCount.count(user) == 0) {
            return 0.0;
        } else {
            size_t usersPage = userToPagesCount.at(user);
            return static_cast<double>(GetLessThanPage(usersPage)) / (userToPagesCount.size() - 1);
        }
    }

    // Вывод доли страниц, которые меньше текущей страницы пользователя
    void Cheer(size_t user) {
        std::cout << std::setprecision(6) << CalculateFractionOfLessPages(user) << std::endl;
    }

private:
    // Проверка, является ли пользователь одиноким на странице
    bool IsUserAlone(int user) {
        return userToPagesCount.count(user) == 1 && userToPagesCount.size() == 1;
    }

    // Получение количества страниц, меньших заданной страницы
    size_t GetLessThanPage(size_t page) {
        size_t result = 0;
        for (size_t i = 0; i < page; ++i) {
            result += pagesToUsers[i];
        }
        return result;
    }

    // Уменьшение счетчика предыдущей страницы пользователя
    void DecreaseCounterPrevUserPage(int user) {
        if (userToPagesCount.count(user) != 0) {
            int prevPage = userToPagesCount.at(user);
            if (prevPage > 0) {
                pagesToUsers[prevPage] = max(0, pagesToUsers[prevPage] - 1);
            }
        }
    }

    // Обновление страницы пользователя
    void UpdateUserPage(int user, int page) {
        userToPagesCount[user] = page;
    }

    // Увеличение счетчика новой страницы пользователя
    void IncreaseCounterNewUserPage(int page) {
        if (page >= 0 && page < pagesToUsers.size()) {
            pagesToUsers[page] += 1;
        }
    }

    std::unordered_map<int, int> userToPagesCount;
    const int MAX_SIZE = 1001;
    std::vector<int> pagesToUsers = std::vector<int>(MAX_SIZE, 0);
};

int main() {
    int query_count;
    cin >> query_count;
    EBook book;

    for (int i = 0; i < query_count; ++i) {
        string query;
        cin >> query;

        if (query == "READ") {
            int user, page;
            cin >> user >> page;
            book.UpdateUserLastPage(user, page);
        } else if (query == "CHEER") {
            int user;
            cin >> user;
            book.Cheer(user);
        }
    }

    return 0;
}
