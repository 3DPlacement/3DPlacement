#ifndef Basedata_H_
#define Basedata_H_

/**
 * Data interface used by SA2
 */
class Base_data
{
    public:
        virtual ~Base_data() {}
        virtual double getans()=0;
        virtual void move()=0;
        virtual void undo()=0;
        virtual void update()=0; 
};

#endif //Basedata_H_
