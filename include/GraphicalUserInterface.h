#ifndef GRAPHICALUSERINTERFACE_H
#define GRAPHICALUSERINTERFACE_H

#pragma once

#include "../include/FileIO.hpp"

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>

int run_gui(int argc, char *argv[]);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onBrowseClicked();
    void onCheckClicked();
    void onPrettifyClicked();
    void onConvertClicked();
    void onMinifyClicked();
    void onCompressClicked();
    void onDecompressClicked();
    void onSaveClicked();
    void printOutput(const std::string& data);

private:
    QLineEdit *inputPath;
    QTextEdit *inputText;
    QTextEdit *outputText;


    FileIO fileIO;
};


#endif