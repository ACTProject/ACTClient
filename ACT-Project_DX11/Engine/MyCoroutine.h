#include <coroutine>
#include <thread>

#pragma once

// co_await(�� ��ȯx ����), co_yield(�� ��ȯo ����), co_return(�ڷ�ƾ �Լ� ����)
class MyCoroutine
{
public:
	// promise_type: �ڷ�ƾ�� ���¸� �����ϰ�, �ڷ�ƾ�� �����ϴ� ������ ��
	struct promise_type
	{
		// �ڷ�ƾ ��ü�� ��ȯ. promise_type�� ����� �ڷ�ƾ �ڵ鷯�� ����
		MyCoroutine get_return_object() { return MyCoroutine{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
		

		// �ڷ�ƾ�� �ʱ� ���¸� ����. �ڷ�ƾ�� �����ϱ� ���� �׻� �ߴ��ϵ��� ����
		std::suspend_always initial_suspend() { return std::suspend_always{}; }

		// �ڷ�ƾ�� ���� ���¸� ����. �ڷ�ƾ�� ����� �� �׻� �ߴ��ϵ��� ����
		std::suspend_always final_suspend() noexcept { return std::suspend_always{}; }

		// co_return�� ����ϴ� ��� ����
		std::suspend_never return_void() { return std::suspend_never{}; }

		void unhandled_exception() { std::exit(1); }
	};
	
	// ������: promise_type���κ��� ������ �ڷ�ƾ �ڵ鷯�� ����
	MyCoroutine(std::coroutine_handle<promise_type> handler) : _handler(handler) {}

	// �Ҹ���: �ڵ鷯�� ��ȿ�ϸ� �ڷ�ƾ �ڵ鷯�� �ı�
	~MyCoroutine() {}

	// �ڵ鷯�� �ܺο��� ������ �� �ֵ��� ����
	std::coroutine_handle<promise_type> GetHandler() { return _handler; }

private:
	std::coroutine_handle<promise_type> _handler;
};

struct AwaitableSleep 
{
	std::chrono::milliseconds duration;
	explicit AwaitableSleep(std::chrono::milliseconds d) : duration(d) {}

	bool await_ready() const noexcept { return false; }
	void await_suspend(std::coroutine_handle<> handle) const
	{
		// �ڵ鷯�� �����ϰ� ĸó�ϱ� ���� shared_ptr ���
		auto handlePtr = std::make_shared<std::coroutine_handle<>>(handle);

		std::thread t([handle, d = duration]() {
			std::this_thread::sleep_for(d);
			if (!handle)
				handle.resume();
			});
		t.detach();
	}
	void await_resume() const noexcept {}
};