#include <queue>
namespace feFrameWork
{
	template <typename MessageType>
	class CMessagequeue
	{
	public:
		CMessagequeue();
		bool PutMessage(MessageType &msg);
		bool GetMessage(MessageType &msg,);
	private:
		std::queue<MessageType> m_queue;
	};
}