#pragma once
class DebugManager
{
	DECLARE_SINGLE(DebugManager);

public:
	bool IsDebugEnabled() const { return _isDebugEnabled; }
	void EnableDebug(bool enable) { _isDebugEnabled = enable; }

    void CreateConsoleWindow();

    void LogVec3ToConsole(const Vec3& vec, const std::string& label);
    void Log(const std::wstring& message, const std::wstring& tag);
	void LogMatrixToConsole(const Matrix& matrix, const std::string& tag);
private:
	bool _isDebugEnabled = false;
};

