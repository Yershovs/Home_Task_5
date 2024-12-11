#include<iostream>
#include <random>

struct ListNode {
    int value;
    ListNode* next;

    ListNode(int val) : value(val), next(nullptr) {}

    // Перегрузка оператора <
    bool operator<(const ListNode& other) const {
        const ListNode* current1 = this;
        const ListNode* current2 = &other;
        while (current1 && current2) {
            if (current1->value != current2->value) {
                return current1->value < current2->value;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        // Если один список короче, он считается меньшим
        return current2 != nullptr;
    }

    // Перегрузка оператора ==
    bool operator==(const ListNode& other) const {
        const ListNode* current1 = this;
        const ListNode* current2 = &other;
        while (current1 && current2) {
            if (current1->value != current2->value) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        // Оба списка равны, если они одинаковой длины
        return current1 == nullptr && current2 == nullptr;
    }
};

  // Настройка генератора случайных чисел
    std::random_device rd;  // Источник случайности
    std::mt19937 gen(rd()); // Генератор Mersenne Twister
    std::uniform_int_distribution<> dist(1, 1000000); // Диапазон 

int generate(void){
    // Генерация случайного числа
    int randomNumber = dist(gen);
    return randomNumber;
}