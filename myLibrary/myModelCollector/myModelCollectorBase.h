#ifndef _MY_MODEL_COLLECTOR_BASE_H_
#define _MY_MODEL_COLLECTOR_BASE_H_

#include "../common.h"
#include <memory>
#include "../myModelIndexer/myModelIndexerBase.h"

class myModelCollectorBase {
public:     // public attribute

protected:  // protected attribute
    std::unique_ptr<myModelIndexerBase> m_poIndexer;

private:    // private attribute

public:     // public method
    myModelCollectorBase(void) {}
    virtual ~myModelCollectorBase(void) {}

protected:  // protected method

private:    // private method

};

#endif // !_MY_MODEL_COLLECTOR_BASE_H_