#ifndef BESKAR_APPLICATION_H
#define BESKAR_APPLICATION_H

class application
{
public:
    application();
    ~application();

    void initialize();
    void run(int width, int height);
    void update(float delta_time);
};

#endif //BESKAR_APPLICATION_H
