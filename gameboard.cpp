#include "gameboard.h"
#include <algorithm>
#include <random>

namespace  {
bool isAdjacent(GameBoard::Position f, GameBoard::Position s) {
    if (f == s)
        return false;
    auto calcDistance = [](const int l, const int r) {
        return std::abs(l - r);
    };
    int distance = calcDistance(f.first, s.first)
            + calcDistance(f.second, s.second);
    return distance == 1;
}
}

GameBoard::GameBoard(const int boardDimension, QObject *parent)
    : QAbstractListModel(parent),
      m_dimension(boardDimension),
      m_boardSize(m_dimension * m_dimension) {
    m_rawBoard.resize(m_boardSize);
    std::iota(m_rawBoard.begin(), m_rawBoard.end(), 1);
    shuffle();
}

int GameBoard::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_rawBoard.size();
}

QVariant GameBoard::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const int rowIndex{index.row()};
    if (!IsPositionValid(rowIndex))
        return {};

    return QVariant::fromValue(m_rawBoard[rowIndex].value);
}

int GameBoard::dimension() const {
    return m_dimension;
}

void GameBoard::shuffle() {
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    do {
       std::shuffle(m_rawBoard.begin(), m_rawBoard.end(), generator);
    } while (!IsBoardValid());
}

bool GameBoard::IsPositionValid(const int position) const {
    return position < m_boardSize;
}

bool GameBoard::IsBoardValid() const {
    int inv = 0;
    for (int i = 0; i < m_boardSize; i++)
        if (m_rawBoard[i].value != 0)
            for (int j = 0; j < i; j++)
                if (m_rawBoard[j].value > m_rawBoard[i].value)
                    ++inv;
    for (int i = 0; i < m_boardSize; i++)
        if (m_rawBoard[i].value == 0)
            inv += 1 + i / m_dimension;
    return (inv % 2 != 1);
}

int GameBoard::boardSize() const {
    return m_boardSize;
}

bool GameBoard::move(int index) {
    if (GameOver() && !IsPositionValid(index))
        return false;

    Position moved = getRowCol(index);

    auto it = std::find_if(m_rawBoard.begin(), m_rawBoard.end(), [=](const Tile& tile) {
        return tile.value == m_boardSize;
    });

    Q_ASSERT(it != m_rawBoard.end());

    int hiddenIndex = it - m_rawBoard.begin();
    Position hidden = getRowCol(hiddenIndex);
    if (!isAdjacent(moved, hidden))
        return false;

    std::swap(m_rawBoard[hiddenIndex], m_rawBoard[index]);
    emit dataChanged(createIndex(0, 0), createIndex(m_boardSize, 0));

    return true;
}

GameBoard::Position GameBoard::getRowCol(int index) const {
    Q_ASSERT(m_dimension > 0);
    int row = index / m_dimension;
    int column = index % m_dimension;
    return std::make_pair(row, column);
}

bool GameBoard::GameOver() const {
    if (std::is_sorted(m_rawBoard.begin(), m_rawBoard.end()))
        return true;
    return false;
}

