from common import component
from ut import ut_func


class R(component.Component):
    pass


class A(component.Component):
    pass


class B(component.Component):
    pass


class C(component.Component):
    pass


class ComponentCase(ut_func.TestCase):
    def setUp(self) -> None:
        self.root = R()
        a = A(self.root)
        a1 = B(a)
        a1.add_component(A(a1))
        a1.add_component(A(a1))
        a.add_component(a1)
        b = B(self.root)
        b1 = A(b)
        b1.add_component(B(b1))
        b1.add_component(C(b1))
        b.add_component(b1)
        self.root.add_component(a)
        self.root.add_component(A(self.root))
        self.root.add_component(b)

    def test_remove(self):
        self.root.remove_component(B)
        self.assertEqual(None, self.root.get_component(B))

    def test_get_comps(self):
        result = self.root.get_components(A)
        self.assertEqual(2, len(result))
        result = self.root.get_components(B)
        self.assertEqual(1, len(result))

    def test_get_comp(self):
        result = self.root.get_component(B)
        self.assertTrue(isinstance(result, B))

    def test_get_comp_in_children(self):
        result = self.root.get_component_in_children(C)
        self.assertTrue(isinstance(result, C))

    def test_get_comps_in_children(self):
        result = self.root.get_components_in_children(A)
        self.assertEqual(3, len(result))

    def test_get_comp_in_parent(self):
        comp = self.root.get_component_in_children(C)
        self.assertTrue(isinstance(comp.get_component_in_parent(A), A))

    def test_get_comps_in_parent(self):
        comp = self.root.get_component_in_children(C)
        self.assertEqual(3, len(comp.get_components_in_parent(A)))

    def test_parent(self):
        self.assertEqual(self.root.parent, None)
        self.assertEqual(self.root.get_component(A).parent, self.root)

    def test_root(self):
        comp = self.root.get_component_in_children(C)
        self.assertEqual(comp.root, self.root)


if __name__ == '__main__':
    ut_func.unittest.main()
