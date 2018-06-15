#include "viewer.hh"

#include <chrono>
#include <thread>

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

    int index = (p[2] * size_x * size_y) + (p[1] * size_x) + p[0];
    double val = vect[index];

    float coeff = val / max_val;
    unsigned char red = static_cast<unsigned char>(255 * coeff);
    unsigned char blue = static_cast<unsigned char>(255 * (1 - coeff));
    unsigned char color[3] = { red, 0, blue };

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
  actor->GetProperty()->SetPointSize(3);

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> render_window_interactor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  render_window_interactor->SetRenderWindow(render_window);

  renderer->AddActor(actor);
  renderer->SetBackground(.0, .0, .0);

  render_window->Render();
  //std::this_thread::sleep_for(std::chrono::seconds(1));
  render_window_interactor->Start();
  render_window_interactor->Initialize();
}
