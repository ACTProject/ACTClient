#pragma once

template <typename T>
class ObjectPool
{
public:
    ObjectPool(size_t poolSize, std::function<shared_ptr<T>()> objectFactory)
        : _objectFactory(objectFactory)
    {
        // 풀 초기화: 미리 오브젝트 생성
        for (size_t i = 0; i < poolSize; ++i)
        {
            auto obj = objectFactory();
            _pool.push(obj);

            obj->SetPool(this);
        }
    }

    shared_ptr<T> Get()
    {
        if (_pool.empty())
        {
            // 풀이 비어있으면 새로운 오브젝트 생성
            auto obj = _objectFactory();
            obj->SetPool(this);
            return obj;
        }

        auto obj = _pool.front();
        _pool.pop();
        return obj;
    }

    void Return(shared_ptr<T> obj)
    {
        _pool.push(obj); // 풀에 반환
    }

private:
    queue<shared_ptr<T>> _pool;                       // 오브젝트 풀
    std::function<shared_ptr<T>()> _objectFactory;    // 오브젝트 생성 함수
};

