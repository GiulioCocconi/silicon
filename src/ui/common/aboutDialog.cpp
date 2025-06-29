/*
 Copyright (c) 2025. Giulio Cocconi
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aboutDialog.hpp"

AboutDialog::AboutDialog(const QString& title, QWidget* parent) : QDialog(parent)
{
  setWindowTitle(QString("About %1").arg(title));
  setFixedSize(600, 500);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  QHBoxLayout* topLayout  = new QHBoxLayout();

  const auto icon = QApplication::windowIcon();
  assert(!icon.isNull());

  const auto titleLabel = new QLabel(title, this);
  titleLabel->setFont(QFont("Chango", 50, QFont::Bold));

  const auto iconLabel = new QLabel(this);
  iconLabel->setPixmap(icon.pixmap(QSize(128, 128)));
  iconLabel->setScaledContents(true);

  topLayout->setSpacing(20);
  topLayout->addWidget(iconLabel, 20);
  topLayout->addWidget(titleLabel, 80);

  mainLayout->addLayout(topLayout);

  auto explainatoryText = new QLabel(
      QString("This software is a part of the SILICON suite (v. %1), developed by "
              "Giulio Cocconi and released under the GNU General Public License "
              "v. 3.0.\n\n It makes use of the following licensed resources:")
          .arg(SILICON_VERSION),
      this);

  explainatoryText->setWordWrap(true);

  mainLayout->addWidget(explainatoryText);

  licenseTextEdit = new QPlainTextEdit(this);
  licenseTextEdit->setReadOnly(true);
  licenseTextEdit->setFont(QFont("NovaMono"));

  closeButton = new QPushButton(tr("Close"), this);
  connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

  mainLayout->addWidget(licenseTextEdit);
  mainLayout->addWidget(closeButton);

  // Load the licenses from the Qt resources
  loadLicenses();
}

void AboutDialog::loadLicenses()
{
  if (this->licenseText.isEmpty()) {
    QDir dir(":/licenses/");

    QStringList fileList = dir.entryList(QDir::Files);

    if (fileList.isEmpty()) {
      licenseTextEdit->setPlainText("Error: License resource directory is empty.");
    }

    for (const QString& fileName : fileList) {
      QString filePath = dir.filePath(fileName);
      QFile   licenseFile(filePath);

      if (licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Add a header for each license to separate them clearly
        this->licenseText.append(
            "=================================================================\n");
        this->licenseText.append("LICENSE: ");
        this->licenseText.append(fileName);
        this->licenseText.append(
            "\n=================================================================\n\n");

        // Append the content of the license file
        QTextStream in(&licenseFile);
        this->licenseText.append(in.readAll());

        // Add some newlines to separate from the next license
        this->licenseText.append("\n\n\n");

        licenseFile.close();
      }

      assert(!this->licenseText.isEmpty());

      licenseTextEdit->setPlainText(this->licenseText);
    }
  }
}