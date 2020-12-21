/*
Штирлиц и Мюллер стреляют по очереди. В очереди n человек, стоящих друг за другом. Каждым выстрелом убивается один из стоящих.
Кроме того, если у кого-то из стоящих в очереди убиты все его соседи, то этот человек в ужасе убегает. Проигрывает тот, кто не может ходить.
Первым стреляет Штирлиц.
Требуется определить, кто выиграет при оптимальной игре обеих сторон, и если победителем будет Штирлиц, то найти все возможные первые ходы, ведущие к его победе. 
Входной файл содержит единственное число n (2≤ n≤ 5 000) — количество человек в очереди. 
Если выигрывает Мюллер, выходной файл должен состоять из единственного слова Mueller.
Иначе в первой строке необходимо вывести слово Schtirlitz, а в последующих строках — номера людей в очереди, которых мог бы первым ходом убить Штирлиц для достижения своей победы.
Номера необходимо выводить в порядке возрастания. 
*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <optional>

std::optional<std::vector<int>> playGame(int N) {
    std::vector<int> SG(N + 1, 0);
    for (int peopleInQueue = 2; peopleInQueue <= N; ++peopleInQueue) {
        std::vector<bool> mex(N, false);
        for (int j = 0; j <= (peopleInQueue - 1) / 2; ++j)
            mex[SG[j] ^ SG[peopleInQueue - j - 1]] = true;
        for (int j = 0; j < N; ++j)
            if (!mex[j]) {
                SG[peopleInQueue] = j;
                break;
            }
    }
    if (SG[N] == 0)
        return std::nullopt;
    else {
        std::vector<int> firstShot;
        for (int personPosition = 1; personPosition <= N; ++personPosition)
            if ((SG[personPosition - 1] ^ SG[N - personPosition]) == 0)
                firstShot.emplace_back(personPosition);
        return firstShot;
    }
}

int main() {
    int N;
    std::cin >> N;
    if (auto firstShot = playGame(N)) {
        std::cout << "Schtirlitz\n";
        for (auto shot : firstShot.value())
            std::cout << shot << ' ';
    }
    else std::cout << "Mueller\n";
    return 0;
}