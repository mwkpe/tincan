#ifndef TIN_CAN_FRAME_TABLE_MODEL_H
#define TIN_CAN_FRAME_TABLE_MODEL_H


#include <cstdint>
#include <vector>
#include <QAbstractTableModel>
#include "canframe.h"


namespace tin
{


static constexpr std::size_t AVG_COUNT = 10;


struct RowData
{
  can::frame can_frame;
  std::uint64_t receive_time;
  std::array<std::int32_t, AVG_COUNT> cycle_times;
  std::size_t cycle_index;
};


class CanFrameTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit CanFrameTableModel(QObject* parent = nullptr);

  QVariant headerData(int section, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const override;
  int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
  int columnCount(const QModelIndex& parent = QModelIndex{}) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  void reset();

public slots:
  void add_frame(std::uint64_t time, can::frame frame);

private:
  std::vector<RowData> rows_;
};


}  // namespace tin


#endif  // TIN_CAN_FRAME_TABLE_MODEL_H
