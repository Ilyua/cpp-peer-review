#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class Domain {
public:
    Domain(const std::string& domain) : domain_(domain) {
        domain_.insert(0, ".");
        reverse(domain_.begin(), domain_.end());
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end(),
                                       [](char lhs, char rhs) -> bool {
                                           if (lhs == rhs && lhs == '.') {
                                               return false;
                                           } else if (lhs == '.') {
                                               return true;
                                           } else if (rhs == '.') {
                                               return false;
                                           } else {
                                               return lhs < rhs;
                                           }
                                       });
    }

    bool IsSubdomain(const Domain& other) const {
        return domain_.size() >= other.domain_.size() &&
               std::equal(other.domain_.begin(), other.domain_.end(), domain_.begin());
    }

    const std::string& GetDomain() const {
        return domain_;
    }

private:
    std::string domain_;
};

class DomainChecker {
public:
    template <typename It>
    DomainChecker(It begin, It end) : forbidden_domains_(begin, end) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto it = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](Domain& lhs, Domain& rhs) -> bool {
            return lhs.IsSubdomain(rhs) or rhs.IsSubdomain(lhs);
        });
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it == forbidden_domains_.begin()) {
            return false;
        }
        it = it - 1;
        return domain.IsSubdomain(*it);
    }

private:
    std::vector<Domain> forbidden_domains_;
};

// Функция для чтения доменов из стандартного ввода
vector<Domain> ReadDomains(istream& in, size_t count) {
    vector<Domain> domains;
    string domain;
    for (size_t i = 0; i < count; ++i) {
        getline(in, domain);
        domains.emplace_back(domain);
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

// Юнит-тесты для класса Domain
void TestDomain() {
    Domain domain1(".example.com");
    Domain domain2(".test.example.com");
    Domain domain3(".example.net");


    assert(domain1 == domain1);
    assert(domain1 < domain2);
    assert(!(domain1 < domain1));
    assert(!(domain2 < domain1));

    assert(!domain1.IsSubdomain(domain3));
}

// Юнит-тесты для класса DomainChecker
void TestDomainChecker() {
    std::vector<Domain> forbidden_domains = {Domain(".example.com"), Domain(".test.example.com")};
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    Domain test_domain1(".example.net");
    Domain test_domain2(".test.example.com");

    assert(!checker.IsForbidden(test_domain1));
    assert(checker.IsForbidden(test_domain2));
}

int main() {
    TestDomain();
    TestDomainChecker();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad" : "Good") << endl;
    }

    return 0;
}

