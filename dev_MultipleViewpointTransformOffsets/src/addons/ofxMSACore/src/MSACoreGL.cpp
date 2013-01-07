#include "MSACore.h"

namespace msa {
	
	static GLfloat tex_coords[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	
	static GLfloat verts[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	
	
	void drawQuadAtCorner() {
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glEnableClientState(GL_VERTEX_ARRAY);		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	
	void drawQuadAtCenter() {
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glEnableClientState(GL_VERTEX_ARRAY);		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	
	
	void drawTexture(GLuint texId, GLenum textureTarget ) {
		glEnable(textureTarget);
		glBindTexture(textureTarget, texId);
		drawQuadAtCenter();
		glDisable(textureTarget);
	}
}