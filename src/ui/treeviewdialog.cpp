#include "treeviewdialog.h"
#include "ui_treeviewdialog.h"


Tree_view_dialog::Tree_view_dialog(const QString& title, QWidget* parent)
    : QDialog{parent}, ui{new Ui::Tree_view_dialog}
{
  ui->setupUi(this);
  ui->treeView->setModel(&model_);
  ui->treeView->header()->setStretchLastSection(true);
  setWindowTitle(title);
}


Tree_view_dialog::~Tree_view_dialog()
{
  delete ui;
}


void Tree_view_dialog::set_data(const std::map<std::int32_t, std::string>& data)
{
  model_.set(data);
}
