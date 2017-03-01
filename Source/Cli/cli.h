/*  Copyright (c) BAVC. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef CLI_H
#define CLI_H
//---------------------------------------------------------------------------

#include "Core/SignalServer.h"
#include "Core/FileInformation.h"
#include "Core/Preferences.h"
#include "Core/FFmpeg_Glue.h"
#include <QCoreApplication>
#include <memory>
#include <iostream>
#include <QTimer>

enum Errors {
    Success = 0,
    NoInput = 1,
    ParsingFailure = 2,
    OutputAlreadyExists = 3,
    InvalidInput = 4
};

class ProgressBar
{
public:
    static const char ForegroundChar = char(178);
    static const char BackgroundChar = char(177);

    ProgressBar(int min, int max, int width, const QString& append) : min(min), max(max), width(width), append(append) {
        setValue(min);
    }

    void setValue(int value) {

        int displayValue = value * width / (max - min);
        int backgroundWidth = width - displayValue;

        if(displayValue > width)
            displayValue = width;

        if(backgroundWidth < 0)
            backgroundWidth = 0;

        std::string foreground = std::string(displayValue, ForegroundChar);
        std::string background = std::string(backgroundWidth, BackgroundChar);

        std::cout << "\r" << foreground << background << " " << value << " of " << max << " " << append.toStdString();
    }


    int getMax() const;
    void setMax(int value);

private:
    int min;
    int max;

    int width;
    QString append;
};


class Cli : public QObject
{
    Q_OBJECT
public:
    Cli();
    int exec(QCoreApplication& a);

public slots:
    void updateParsingProgress();
    void updateExportingProgress();
    void onStatsFileGenerationProgress(int written, int total);

private:
    std::unique_ptr<FileInformation> info;
    QTimer progressTimer;
    std::unique_ptr<ProgressBar> progress;

    int indexOfStreamWithKnownFrameCount;

    int statsFileBytesWritten;
    int statsFileBytesTotal;
};

#endif // 
