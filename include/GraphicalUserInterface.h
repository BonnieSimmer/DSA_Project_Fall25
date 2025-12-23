#ifndef GRAPHICALUSERINTERFACE_H
#define GRAPHICALUSERINTERFACE_H

#pragma once

#include <qboxlayout.h>

#include "FileIO.hpp"
#include "XMLFormatter.hpp"
#include "XMLMinifier.hpp"
#include "XMLCompressor.hpp"
#include "XMLDecompression.hpp"
#include "XMLToJson.hpp"
#include "GraphVisualizer.h"
#include "AppMode.h"
#include "PostSearch.hpp"

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>

#include "NetworkAnalyzer.hpp"
#include "XMLParsser.hpp"

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

    void setupLevel1Layout(QVBoxLayout *layout);
    void setupLevel2Layout(QVBoxLayout *layout);

private slots:
    void onBrowseClicked();
    void onCheckClicked();

    void highlightErrorLine(int lineNumber);

    void onPrettifyClicked();
    void onConvertClicked();
    void onMinifyClicked();
    void onCompressClicked();
    void onDecompressClicked();
    void onSaveClicked();
    void printOutput(const std::string& data) const;
    void saveOutputToFile(const std::string& data, bool compress = false, const std::string& outputFileType = "Compressed Files (*.comp)");

    void onMostInfluencerClicked();
    void onMostActiveClicked();
    void onMutualClicked();
    void onSuggestClicked();
    void onSearchClicked();
    void onDrawGraphClicked();

    bool validateAndFixXML(QString &content);

private:
    std::unique_ptr<PostSearch> searchEngine;
    PostSearch* getReadySearchEngine();
    std::unique_ptr<NetworkAnalyzer> analyzer;
    NetworkAnalyzer *getReadyAnalyzer();
    bool isDataDirty = true;
    QString lastValidationReport;
    bool lastValidationResult = false;
    QString lastParsedContent;

    QLineEdit *inputPath;
    QTextEdit *inputText;
    QTextEdit *outputText;

    QLineEdit *idInput;
    QLineEdit *searchInput;
    QRadioButton *wordRadio;
    QRadioButton *topicRadio;

    XMLCompressor compressor;
    XMLDecompressor decompressor;
    Type type;
};


#endif