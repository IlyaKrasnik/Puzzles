#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QAbstractListModel>
#include <vector>

class GameBoard : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)
    Q_PROPERTY(int hiddenElementIndex READ boardSize CONSTANT)
public:
    using Position = std::pair<int,int>;
    static const int defaultPuzzleDimension = 4;
    GameBoard(const int boardDimension = defaultPuzzleDimension,
              QObject* parent = nullptr);

    struct Tile {
        int value {};
        Tile&operator=(const int rhs) {
            value = rhs;
            return *this;
        }
        bool operator== (const int rhs) const {
            return value == rhs;
        }
        bool operator< (const Tile& rhs) const {
            return value < rhs.value;
        }
    };

    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int dimension() const;
    int boardSize() const;

    Q_INVOKABLE bool move(int index);

private:
    void shuffle();
    bool IsPositionValid(const int position) const;
    bool IsBoardValid() const;
    Position getRowCol(int index) const;
    bool GameOver() const;
    std::vector<Tile> m_rawBoard;
    const int m_dimension;
    const int m_boardSize;
};

#endif // GAMEBOARD_H
