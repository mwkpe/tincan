#include "ui/mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setStyle(QStyleFactory::create("Fusion"));

  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  QPalette dark;
  dark.setColor(QPalette::Window, QColor(53, 53, 53));
  dark.setColor(QPalette::WindowText, Qt::white);
  dark.setColor(QPalette::Base, QColor(25, 25, 25));
  dark.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  dark.setColor(QPalette::ToolTipBase, Qt::white);
  dark.setColor(QPalette::ToolTipText, Qt::white);
  dark.setColor(QPalette::Text, Qt::white);
  dark.setColor(QPalette::Button, QColor(53, 53, 53));
  dark.setColor(QPalette::ButtonText, Qt::white);
  dark.setColor(QPalette::BrightText, Qt::red);
  dark.setColor(QPalette::Link, QColor(42, 130, 218));
  dark.setColor(QPalette::Highlight, QColor(51,231,247));
  dark.setColor(QPalette::HighlightedText, Qt::black);
  a.setPalette(dark);

  Main_window w;
  w.show();

  return a.exec();
}
