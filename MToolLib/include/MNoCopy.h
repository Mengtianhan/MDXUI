#ifndef MNOCOPY_H
#define MNOCOPY_H


class MNoCopy{
public:
    MNoCopy(){}
    virtual ~MNoCopy(){}
private:
    MNoCopy(const MNoCopy&) = delete;
    MNoCopy& operator=(const MNoCopy&)=delete;
};

#endif // MNOCOPY_H
