#include "rt.h"

void	edit_print_sphere(t_object *obj)
{
	(void)obj;
	printf("sphere:\n");
	printf("  WA / SD / TG : move the sphere.\n");
	printf("  Z / X        : scale the radius (diameter).\n");
	printf("  P            : print further info on the object.\n\n");
}

void	edit_print_plane(t_object *obj)
{
	(void)obj;
	printf("plane:\n");
	printf("  WA / SD / TG : move the plane.\n");
	printf("  K / ;        : yaw (rotate around Y axis).\n");
	printf("  O / L        : rotate around camera right.\n");
	printf("  P            : print further info on the object.\n\n");
}

void	edit_print_cylinder(t_object *obj)
{
	(void)obj;
	printf("cylinder:\n");
	printf("  WA / SD / TG : move the cylinder.\n");
	printf("  K / ;        : yaw (rotate around Y axis).\n");
	printf("  O / L        : rotate around camera right.\n");
	printf("  Z / X        : scale the radius.\n");
	printf("  C / V        : scale the height.\n");
	printf("  P            : print further info on the object.\n\n");
}

void	edit_print_cone(t_object *obj)
{
	(void)obj;
	printf("cone:\n");
	printf("  WA / SD / TG : move the cone.\n");
	printf("  K / ;        : yaw (rotate around Y axis).\n");
	printf("  O / L        : rotate around camera right.\n");
	printf("  Z / X        : scale the height.\n");
	printf("  C / V        : scale the angle.\n");
	printf("  P            : print further info on the object.\n\n");
}

void	edit_print_triangle(t_object *obj)
{
	(void)obj;
	printf("triangle:\n");
	printf("  WA / SD / TG : move the triangle.\n");
	printf("  K / ;        : yaw (rotate around Y axis).\n");
	printf("  O / L        : rotate around camera right.\n");
	printf("  Z / X        : uniform scale around center.\n");
	printf("  P            : print further info on the object.\n\n");
}
