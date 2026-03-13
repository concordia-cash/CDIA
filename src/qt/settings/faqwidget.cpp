// Copyright (c) 2019-2022 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/concordia-config.h"
#endif

#include "qt/settings/faqwidget.h"
#include "qt/settings/forms/ui_faqwidget.h"

#include "mnmodel.h"
#include "qtutils.h"

#include <QScrollBar>
#include <QMetaObject>

SettingsFaqWidget::SettingsFaqWidget(PIVXGUI* parent, MNModel* mnModel) :
    QDialog(parent),
    ui(new Ui::SettingsFaqWidget)
{
    ui->setupUi(this);
    this->setStyleSheet(parent->styleSheet());

#ifdef Q_OS_MAC
    ui->container->load("://bg-welcome");
    setCssProperty(ui->container, "container-welcome-no-image");
#else
    setCssProperty(ui->container, "container-welcome");
#endif
    setCssProperty(ui->labelTitle, "text-title-faq");
    setCssProperty(ui->labelWebLink, "text-content-white");

    // Content
    setCssProperty({
           ui->labelNumber_Intro,
           ui->labelNumber_UnspendablePIV,
           ui->labelNumber_Stake,
           ui->labelNumber_Support,
        }, "container-number-faq");

    setCssProperty({
              ui->labelSubtitle_Intro,
              ui->labelSubtitle_UnspendablePIV,
              ui->labelSubtitle_Stake,
              ui->labelSubtitle_Support,
            }, "text-subtitle-faq");


    setCssProperty({
              ui->labelContent_Intro,
              ui->labelContent_UnspendablePIV,
              ui->labelContent_Stake,
              ui->labelContent_Support,
            }, "text-content-faq");


    setCssProperty({
              ui->pushButton_Intro,
              ui->pushButton_UnspendablePIV,
              ui->pushButton_Stake,
              ui->pushButton_Support,
            }, "btn-faq-options");

    ui->labelContent_Support->setOpenExternalLinks(true);

    // Set FAQ content strings
    QString introContent = formatFAQContent(
        formatFAQParagraph(
            tr("Concordia Cash is a form of digital online money using blockchain technology "
               "that can be easily transferred globally, instantly, and with near "
               "zero fees. Concordia Cash incorporates market leading security & "
               "privacy and is also the first PoS (Proof of Stake) Cryptocurrency "
               "to implement Sapling(SHIELD), a zk-SNARKs based privacy protocol.")) +
        formatFAQParagraph(
            tr("Concordia Cash utilizes a Proof of Stake (PoS) consensus system algorithm, "
               "allowing all owners of Concordia Cash to participate in earning block rewards "
               "while securing the network with full node wallets, as well as to "
               "run Masternodes to create and vote on proposals.")));
    ui->labelContent_Intro->setText(introContent);

    QString unspendablePIVContent = formatFAQContent(
        formatFAQParagraph(
            tr("Newly received Concordia Cash requires 6 confirmations on the network "
               "to become eligible for spending which can take ~6 minutes.")) +
        formatFAQParagraph(
            tr("Your Concordia Cash wallet also needs to be completely synchronized "
               "to see and spend balances on the network.")));
    ui->labelContent_UnspendablePIV->setText(unspendablePIVContent);

    QString stakeContent = formatFAQContent(
        formatFAQOrderedList(
            formatFAQListItem(tr("Make sure your wallet is completely synchronized and you are using the latest release.")) +
            formatFAQListItem(tr("You must have a balance of Concordia Cash with a minimum of 600 confirmations.")) +
            formatFAQListItem(tr("Your wallet must stay online and be unlocked for staking purposes.")) +
            formatFAQListItem(tr("Once all those steps are followed staking should be enabled."))) +
        formatFAQParagraph(
            tr("You can see the status of staking in the wallet by mousing over the "
               "package icon in the row on the top left of the wallet interface. The "
               "package will be lit up and will state \"Staking Enabled\" to indicate "
               "it is staking. Using the command line interface (%1); the command %2 "
               "will confirm that staking is active.")
                .arg("concordia-cli", "<span style=\"font-style:italic\">getstakingstatus</span>")));
    ui->labelContent_Stake->setText(stakeContent);

    QString supportContent = formatFAQContent(
        formatFAQParagraph(
            tr("We have support channels in most of our official chat groups, for example %1")
                .arg("<a style='color: #b088ff' href='https://discord.gg/jXUpZVMZFC'>" + tr("#support in our Discord") + "</a>.")));
    ui->labelContent_Support->setText(supportContent);

    // Exit button
    setCssProperty(ui->pushButtonExit, "btn-faq-exit");

    // Web Link
    setCssProperty(ui->pushButtonWebLink, "btn-faq-web");
    setCssProperty(ui->containerButtons, "container-faq-buttons");

    // Buttons
    connect(ui->pushButtonExit, &QPushButton::clicked, this, &SettingsFaqWidget::close);
    connect(ui->pushButton_Intro, &QPushButton::clicked, [this](){onFaqClicked(ui->widget_Intro);});
    connect(ui->pushButton_UnspendablePIV, &QPushButton::clicked, [this](){onFaqClicked(ui->widget_UnspendablePIV);});
    connect(ui->pushButton_Stake, &QPushButton::clicked, [this](){onFaqClicked(ui->widget_Stake);});
    connect(ui->pushButton_Support, &QPushButton::clicked, [this](){onFaqClicked(ui->widget_Support);});

    if (parent)
        connect(parent, &PIVXGUI::windowResizeEvent, this, &SettingsFaqWidget::windowResizeEvent);
}

void SettingsFaqWidget::showEvent(QShowEvent *event)
{
    QPushButton* btn = getButtons()[section];
    QMetaObject::invokeMethod(btn, "setChecked", Qt::QueuedConnection, Q_ARG(bool, true));
    QMetaObject::invokeMethod(btn, "clicked", Qt::QueuedConnection);
}

void SettingsFaqWidget::setSection(Section _section)
{
    section = _section;
}

void SettingsFaqWidget::onFaqClicked(const QWidget* const widget)
{
    ui->scrollAreaFaq->verticalScrollBar()->setValue(widget->y());
}

void SettingsFaqWidget::windowResizeEvent(QResizeEvent* event)
{
    QWidget* w = qobject_cast<QWidget*>(parent());
    this->resize(w->width(), w->height());
    this->move(QPoint(0, 0));
}

std::vector<QPushButton*> SettingsFaqWidget::getButtons()
{
    return {
            ui->pushButton_Intro,
            ui->pushButton_UnspendablePIV,
            ui->pushButton_Stake,
            ui->pushButton_Support,
    };
}

SettingsFaqWidget::~SettingsFaqWidget()
{
    delete ui;
}


