#include "canframetablemodel.h"


#include <algorithm>
#include <QFont>


namespace
{


inline std::int32_t avg(const std::array<std::int32_t, tin::AVG_COUNT>& cycle_times)
{
  return std::accumulate(std::begin(cycle_times), std::end(cycle_times),
      static_cast<std::int32_t>(0)) / tin::AVG_COUNT;
}


inline std::size_t next(std::size_t& index)
{
  if (++index >= tin::AVG_COUNT)
    index = 0;
  return index;
}


}  // namespace


tin::CanFrameTableModel::CanFrameTableModel(QObject *parent) : QAbstractTableModel{parent}
{
}


QVariant tin::CanFrameTableModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0: return "ID";
      case 1: return "Avg. Cycle";
      case 2: return "DLC";
      case 3: return "Data";
    }
  }

  return QVariant{};
}


int tin::CanFrameTableModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return rows_.size();
}


int tin::CanFrameTableModel::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 4;
}


QVariant tin::CanFrameTableModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};

  if (role == Qt::DisplayRole) {
    const auto& row = rows_.at(index.row());
    switch (index.column()) {
      case 0: return QString::number(row.can_frame.id, 16).toUpper();
      case 1: return QString::number(avg(row.cycle_times) / 1000.0, 'f', 3);
      case 2: return row.can_frame.dlc;
      case 3: {
        QString hex_data;
        for (auto b : row.can_frame.data) {
          hex_data += QString::number(b, 16).rightJustified(2, '0').toUpper();
          hex_data += " ";
        }
        return hex_data;
      }
    }
  }

  if (role == Qt::TextAlignmentRole) {
    if (index.column() == 3)
      return Qt::AlignLeft + Qt::AlignVCenter;
    return Qt::AlignCenter;
  }

  if (role == Qt::FontRole) {
    return QFont("Consolas", 11);
  }

  return QVariant{};
}


void tin::CanFrameTableModel::reset()
{
  beginResetModel();
  rows_.clear();
  endResetModel();
  dbc_file_ = dbc::file{};
}


void tin::CanFrameTableModel::add_dbc(dbc::file&& dbc_file)
{
  dbc_file_ = std::move(dbc_file);
}


void tin::CanFrameTableModel::add_frame(uint64_t time, can::frame frame)
{
  auto it = std::find_if(std::begin(rows_), std::end(rows_), [&frame](const auto& row) {
    return row.can_frame.id == frame.id;
  });
  if (it != std::end(rows_)) {  // Update row
    auto row_index = std::distance(std::begin(rows_), it);
    auto& row = rows_[row_index];
    row.can_frame = frame;
    row.cycle_times[next(row.cycle_index)] = time - row.receive_time;
    row.receive_time = time;
    dataChanged(index(row_index, 0), index(row_index, 3));
  }
  else {  // Insert row
    beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    rows_.emplace_back();
    auto& row = rows_.back();
    row.can_frame = frame;
    row.receive_time = time;
    endInsertRows();
  }
}
