#include "viewer.hh"

void viewer(const std::vector<double>& vect, int size_x, int size_y, int size_z, double max_val)
{
  vtkSmartPointer<vtkStructuredGrid> structured_grid = vtkSmartPointer<vtkStructuredGrid>::New();

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

  for (int i = 0; i < size_x; ++i)
    for (int j = 0; j < size_y; ++j)
      for (int k = 0; k < size_z; ++k)
        points->InsertNextPoint(i, j, k);

  structured_grid->SetDimensions(size_x, size_y, size_z);
  structured_grid->SetPoints(points);

  vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("colors");

  for (unsigned int i = 0; i < structured_grid->GetNumberOfPoints(); ++i)
  {
    double p[3];
    structured_grid->GetPoint(i, p);

    double x_val = vect[p[0]];
    double y_val = vect[p[1]];
    double z_val = vect[p[2]];

    double dcol[3] = { x_val/max_val, y_val/max_val, z_val/max_val };

    unsigned char color[3];
    for (unsigned int j = 0; j < 3; ++j)
      color[j] = static_cast<unsigned char>(255 * dcol[j]);

#if VTK_MAJOR_VERSION > 7
    colors->InsertNextTypedTuple(color);
#else
    colors->InsertNextTupleValue(color);
#endif
  }

  structured_grid->GetPointData()->SetScalars(colors);

  vtkSmartPointer<vtkStructuredGridGeometryFilter> geometry_filter =
    vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  geometry_filter->SetInputConnection(structured_grid->GetProducerPort());
#else
  geometry_filter->SetInputData(structured_grid);
#endif
  geometry_filter->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(geometry_filter->GetOutputPort());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(5);

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> render_window_interactor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  render_window_interactor->SetRenderWindow(render_window);

  renderer->AddActor(actor);
  renderer->SetBackground(.0, .0, .0);

  render_window->Render();
  render_window_interactor->Start();
}
