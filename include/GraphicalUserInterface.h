#ifndef GRAPHICALUSERINTERFACE_H
#define GRAPHICALUSERINTERFACE_H

#pragma once

#include "FileIO.hpp"
#include "XMLFormatter.hpp"
#include "XMLMinifier.hpp"
#include "XMLCompressor.hpp"
#include "XMLDecompression.hpp"
#include "XMLToJson.hpp"

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>

enum Type {
    XML,
    JSON,
    OTHER
};

int run_gui(int argc, char *argv[]);

class MainWindow : public QMainWindow {
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
    void printOutput(const std::string& data) const;
    void saveOutputToFile(const std::string& data, bool compress = false, const std::string& outputFileType = "Compressed Files (*.comp)");

private:
    QLineEdit *inputPath;
    QTextEdit *inputText;
    QTextEdit *outputText;

    XMLCompressor compressor;
    XMLDecompressor decompressor;
    Type type;
};


#endif