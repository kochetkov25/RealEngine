#include "IndexBuffer.h"

namespace Render {

/*деструктор*/
IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &_id); }

/*сделать буффер активным*/
void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

/*отвязать буффер*/
void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace Render