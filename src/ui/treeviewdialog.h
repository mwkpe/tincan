#ifndef TIN_TREEVIEWDIALOG_H
#define TIN_TREEVIEWDIALOG_H


#include <cstdint>
#include <string>
#include <map>

#include <QDialog>

#include "models/valuedefmodel.h"


namespace Ui { class Tree_view_dialog; }


class Tree_view_dialog : public QDialog
{
  Q_OBJECT

public:
  Tree_view_dialog(const QString& title, QWidget* parent = nullptr);
  ~Tree_view_dialog();
  void set_data(const std::map<std::int32_t, std::string>& data);

private:
  Ui::Tree_view_dialog* ui = nullptr;
  tin::Value_def_model model_;
};


#endif  // TIN_TREEVIEWDIALOG_H
