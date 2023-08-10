#define DGE_APPLICATION
#include "defGameEngine.h"

constexpr int32_t NODE_SIZE = 12;

struct Shape;

struct Node
{
	Shape* parent;
	def::vi2d pos;
};

struct Shape
{
	std::vector<Node> nodes;

	size_t maxNodes;
	bool expired = false;

	virtual void DrawYourself(def::GameEngine* pge) = 0;

	Node* GetNextNode(const def::vf2d& pos)
	{
		if (nodes.size() == maxNodes)
			return nullptr;

		Node n;
		n.parent = this;
		n.pos = pos;
		nodes.push_back(n);

		return &nodes.back();
	}

	void DrawNodes(def::GameEngine* pge)
	{
		for (const auto& n : nodes)
			pge->FillCircle(n.pos, 2, def::RED);
	}

	Node* HitNode(const def::vi2d& vPos)
	{
		for (auto& n : nodes)
		{
			if (n.pos == vPos)
				return &n;
		}

		return nullptr;
	}
};

struct Line : Shape
{
	Line()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::GameEngine* pge) override
	{
		pge->FillCircle(nodes[0].pos, 2, def::RED);
		pge->FillCircle(nodes[1].pos, 2, def::RED);

		pge->DrawLine(nodes[0].pos, nodes[1].pos, def::WHITE);
		pge->DrawLine(nodes[0].pos, nodes[1].pos);
	}
};

struct Circle : Shape
{
	Circle()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::GameEngine* pge) override
	{
		pge->DrawLine(nodes[0].pos, nodes[1].pos, def::WHITE);

		uint32_t radius = uint32_t((nodes[1].pos - nodes[0].pos).mag());
		pge->DrawCircle(nodes[0].pos, radius);
	}
};

struct Rect : Shape
{
	Rect()
	{
		nodes.reserve(2);
		maxNodes = 2;
	}

	virtual void DrawYourself(def::GameEngine* pge) override
	{
		def::vi2d p1 = nodes[0].pos;
		def::vi2d p2 = nodes[1].pos;
		if (p1 > p2) std::swap(p1, p2);

		pge->DrawRectangle(p1, p2 - p1, def::WHITE);
	}
};

struct Curve : Shape
{
	Curve()
	{
		nodes.reserve(3);
		maxNodes = 3;
	}

	virtual void DrawYourself(def::GameEngine* pge) override
	{
		if (nodes.size() == 2)
			pge->DrawLine(nodes[0].pos, nodes[1].pos, def::WHITE);

		if (nodes.size() == 3)
		{
			pge->DrawLine(nodes[0].pos, nodes[1].pos, def::WHITE);
			pge->DrawLine(nodes[1].pos, nodes[2].pos, def::WHITE);

			def::vi2d op = nodes[0].pos;
			def::vi2d np = op;

			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				np = (1 - t) * (1 - t) * nodes[0].pos + 2 * (1 - t) * t * nodes[1].pos + t * t * nodes[2].pos;
				pge->DrawLine(op, np);
				op = np;
			}
		}
	}
};

class SimpleCAD : public def::GameEngine
{
public:
	SimpleCAD()
	{
		SetTitle("SimpleCAD");
	}

	virtual ~SimpleCAD()
	{
		for (auto& shape : shapes)
			delete shape;
	}

public:
	void DrawSelectedArea()
	{
		if (selectedArea.second != def::vi2d(-1, -1))
		{
			def::vi2d first = selectedArea.first;
			def::vi2d second = selectedArea.second;

			if (first > second)
				std::swap(first, second);

			SetPixelMode(def::Pixel::ALPHA);
			FillRectangle(first, second - first, def::Pixel(255, 255, 255, 122));
			SetPixelMode(def::Pixel::DEFAULT);
		}
	}

	bool OnUserCreate() override
	{
		top.x = ScreenWidth() / 2 - (ScreenWidth() / 2) % NODE_SIZE;
		top.y = 0;

		bottom.x = top.x;
		bottom.y = ScreenHeight();

		left.x = 0;
		left.y = ScreenHeight() / 2 - (ScreenHeight() / 2) % NODE_SIZE;

		right.x = ScreenWidth();
		right.y = left.y;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(def::DARK_BLUE);

		def::vi2d cursor;
		cursor.x = MouseX() - MouseX() % NODE_SIZE;
		cursor.y = MouseY() - MouseY() % NODE_SIZE;

		if (GetKey(def::Key::L).pressed)
		{
			tempShape = new Line();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::C).pressed)
		{
			tempShape = new Circle();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::R).pressed)
		{
			tempShape = new Rect();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetKey(def::Key::B).pressed)
		{
			tempShape = new Curve();

			selected = tempShape->GetNextNode(cursor);
			selected = tempShape->GetNextNode(cursor);
		}

		if (GetMouse(1).pressed)
		{
			for (const auto& shape : shapes)
			{
				Node* node = shape->HitNode(cursor);

				if (node)
				{
					selected = node;
					break;
				}
			}

			if (!selected)
			{
				selectedArea.first = cursor;
				selectedArea.second = { -1, -1 };
			}
		}

		if (GetMouse(1).held)
		{
			if (!selected)
				selectedArea.second = cursor;
		}

		if (GetKey(def::Key::D).pressed)
		{
			if (selectedArea.second != def::vi2d(-1, -1))
			{
				for (int32_t x = selectedArea.first.x; x <= selectedArea.second.x; x += NODE_SIZE)
					for (int32_t y = selectedArea.first.y; y <= selectedArea.second.y; y += NODE_SIZE)
					{
						for (const auto& shape : shapes)
						{
							Node* node = shape->HitNode({ x, y });

							if (node)
								node->parent->expired = true;
						}
					}

				selectedArea.first = { -1, -1 };
				selectedArea.second = { -1, -1 };
			}

			if (tempShape)
			{
				delete tempShape;
				tempShape = nullptr;
			}
			else
			{
				if (selected)
				{
					selected->parent->expired = true;
					selected = nullptr;
				}
			}
		}

		if (selected)
			selected->pos = cursor;

		if (GetMouse(0).released)
		{
			if (tempShape)
			{
				selected = tempShape->GetNextNode(cursor);
				if (!selected)
				{
					shapes.push_back(tempShape);
					tempShape = nullptr;
				}
			}
			else
				selected = nullptr;
		}

		DrawCircle(cursor, 2, def::DARK_GREY);

		for (int32_t x = 0; x < ScreenWidth(); x += NODE_SIZE)
			for (int32_t y = 0; y < ScreenHeight(); y += NODE_SIZE)
				Draw(x, y, def::DARK_BLUE);

		DrawLine(top, bottom, def::GREY);
		DrawLine(left, right, def::GREY);

		for (size_t i = 0; i < shapes.size(); i++)
		{
			auto shape = shapes[i];

			if (shape->expired)
			{
				shapes.erase(shapes.begin() + i);
				continue;
			}

			shape->DrawYourself(this);
			shape->DrawNodes(this);
		}

		if (tempShape)
		{
			tempShape->DrawYourself(this);
			tempShape->DrawNodes(this);
		}

		DrawSelectedArea();

		return true;
	}

private:
	std::vector<Shape*> shapes;

	def::vi2d top, bottom, left, right;

	Shape* tempShape = nullptr;
	Node* selected = nullptr;

	std::pair<def::vi2d, def::vi2d> selectedArea;

};

int main()
{
	SimpleCAD demo;

	demo.Construct(1280, 720, 1, 1);
	demo.Run();

	return 0;
}
