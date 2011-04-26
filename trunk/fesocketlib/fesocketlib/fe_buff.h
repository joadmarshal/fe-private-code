#ifndef _FE_FESL__BUFF
#define _FE_FESL__BUFF
namespace fesl
{
	class CBuff
	{
	public:
		CBuff(size_t initsize=1)
		{
			m_allocasize=initsize;
			m_buff=new byte[m_allocasize+1]+1;
			++m_buff;
			m_buff[-1]=1;
			m_size=0;
		}
		
		CBuff(byte* srcbuff,size_t size)
		{
			m_allocasize=size;
			m_buff=new byte[m_allocasize+1]+1;
			m_buff[-1]=1;
			m_size = size;
		}

		CBuff(CBuff &srcbuff)
		{
			this->m_allocasize = srcbuff.m_allocasize;
			this->m_buff = srcbuff.m_buff;
			this->m_size = srcbuff.m_size;
			AddRef();
		}

		~CBuff()
		{
			Release();
		}

		const byte *Add(void *buff,size_t size)
		{
			if(GetRefCount()>1 || (m_size + size > m_allocasize))
			{
				Release();
				ReNew((m_size + size)*2);
			}
			memcpy(m_buff+m_size,buff,size);
			m_size+=size;
			return m_buff;
		}

		void Clear()
		{
			if(GetRefCount()>1)
			{
				Release();
				ReNew(m_allocasize);
			}
			else
			{
				m_size=0;
			}
		}

		const byte *GetBuff()
		{
			return m_buff;
		}

		size_t size()
		{
			return m_size;
		}

		
	private:
		void AddRef()
		{
			m_buff[-1]=m_buff[-1]+1;
		}

		void Release()
		{
			m_buff[-1]=m_buff[-1]-1;
			if(m_buff[-1]==0)
			{
				delete [](m_buff-1);
			}
		}

		void ReNew(size_t newsize)
		{
			byte *_buff=new byte[newsize+1];
			++_buff;
			memcpy(_buff,m_buff,m_size);
			_buff[-1]=0;
			m_allocasize=newsize;
			m_buff=_buff;
			m_buff[-1]=1;
		}
		byte GetRefCount()
		{
			return m_buff[-1];
		}
		byte *m_buff;
		size_t m_size;//使用的大小
		size_t m_allocasize;//堆空间
	};
}
#endif