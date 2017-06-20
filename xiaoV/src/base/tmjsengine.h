#ifndef TMJSENGINE_H
#define TMJSENGINE_H

/*
 * Manage Main js engine and Module js engine .
 *
 */

#include <QObject>
#include <tmjsenginework.h>

class TmJsEngine : public QObject
{
    Q_OBJECT
public:
    explicit TmJsEngine(QObject *parent = 0);

    void exit();
signals:
    void sig_js_event(QString event, QString val);
public slots:
    void initConfig();
    bool load_js_main_by_name(QString strName);
    bool load_js_module_by_name(QString strName);
    bool load_js_main_content(QString strJsContent);
    bool load_js_module_content(QString strJsContent);
    bool load_js_main_isRunning();
    bool load_js_module_isRunning();
    void load_js_main_stop();
    void load_js_module_stop();

};

#endif // TMJSENGINE_H
