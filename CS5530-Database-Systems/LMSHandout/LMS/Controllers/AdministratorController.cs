using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.Json;
using System.Threading.Tasks;
using LMS.Models.LMSModels;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860
[assembly: InternalsVisibleTo("LMSControllerTests")]
namespace LMS.Controllers
{
    public class AdministratorController : Controller
    {
        private readonly LMSContext db;

        public AdministratorController(LMSContext _db)
        {
            db = _db;
        }

        // GET: /<controller>/
        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Department(string subject)
        {
            ViewData["subject"] = subject;
            return View();
        }

        public IActionResult Course(string subject, string num)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            return View();
        }

        /*******Begin code to modify********/

        /// <summary>
        /// Create a department which is uniquely identified by it's subject code
        /// </summary>
        /// <param name="subject">the subject code</param>
        /// <param name="name">the full name of the department</param>
        /// <returns>A JSON object containing {success = true/false}.
        /// false if the department already exists, true otherwise.</returns>
        public IActionResult CreateDepartment(string subject, string name)
        {
            try
            {
                // check the input subject is less than or equal to 4 chars
                if (subject.Length > 4 || subject.Length < 1)
                {
                    // Return a JavaScript alert with the exception message
                    return Content("Subject code must be 4 characters or less.");
                }

                // check if a department with a input subject that already exists
                var existingDepart = db.Departments.FirstOrDefault(d => d.Subject == subject);
                //false if the department already exists
                if (existingDepart != null)
                {
                    return Content("the department already exists");
                    //return Json(new { success = false });
                }
                else
                {
                    var newDepart = new Department
                    {
                        Subject = subject,
                        Name = name
                    };

                    db.Departments.Add(newDepart);
                    db.SaveChanges();

                    return Json(new { success = true });
                }
            }
            catch (Exception ex)
            {
                return Json(new { success = false, message = ex.Message });
            }
        }


        /// <summary>
        /// Returns a JSON array of all the courses in the given department.
        /// Each object in the array should have the following fields:
        /// "number" - The course number (as in 5530)
        /// "name" - The course name (as in "Database Systems")
        /// </summary>
        /// <param name="subjCode">The department subject abbreviation (as in "CS")</param>
        /// <returns>The JSON result</returns>
        public IActionResult GetCourses(string subject)
        {
            var query = from c in db.Courses
                        where c.Depart.Subject == subject
                        select new
                        {
                            number = c.Number,
                            name = c.Name
                        };

            return Json(query.ToArray());
        }

        /// <summary>
        /// Returns a JSON array of all the professors working in a given department.
        /// Each object in the array should have the following fields:
        /// "lname" - The professor's last name
        /// "fname" - The professor's first name
        /// "uid" - The professor's uid
        /// </summary>
        /// <param name="subject">The department subject abbreviation</param>
        /// <returns>The JSON result</returns>
        public IActionResult GetProfessors(string subject)
        {
            var query = from p in db.Professors
                        join d in db.Departments on p.DepartId equals d.DepartId
                        where d.Subject == subject
                        select new
                        {
                            lname = p.FName,
                            fname = p.FName,
                            uid = p.UId
                        };

            return Json(query.ToArray());

        }



        /// <summary>
        /// Creates a course.
        /// A course is uniquely identified by its number + the subject to which it belongs
        /// </summary>
        /// <param name="subject">The subject abbreviation for the department in which the course will be added</param>
        /// <param name="number">The course number</param>
        /// <param name="name">The course name</param>
        /// <returns>A JSON object containing {success = true/false}.
        /// false if the course already exists, true otherwise.</returns>
        public IActionResult CreateCourse(string subject, int number, string name)
        {
            try
            {
                // check the input subject is less than or equal to 4 chars
                if (number > 9999 || number < 1)
                {
                    // Return a JavaScript alert with the exception message
                    return Content("Subject code must be 5 digits or less.");
                }

                //find department id and check department
                var department = db.Departments.FirstOrDefault(d => d.Subject == subject);
                if (department == null)
                {
                    return Content("the subject you enter is not exist");
                }

                // check if a department with a input subject that already exists
                var existingCourse = db.Courses.FirstOrDefault(c => c.Number == number && c.Depart.Subject == subject);

                //false if the department already exists
                if (existingCourse != null)
                {
                    return Content("this course already exist");
                    //return Json(new { success = false });
                }
                else
                {
                    var newCourse = new Course
                    {
                        Name = name,
                        Number = (ushort)number,
                        DepartId = department.DepartId
                    };

                    db.Courses.Add(newCourse);
                    db.SaveChanges();

                    return Json(new { success = true });
                }
            }
            catch (Exception ex)
            {
                return Json(new { success = false, message = ex.Message });
            }
        }



        /// <summary>
        /// Creates a class offering of a given course.
        /// </summary>
        /// <param name="subject">The department subject abbreviation</param>
        /// <param name="number">The course number</param>
        /// <param name="season">The season part of the semester</param>
        /// <param name="year">The year part of the semester</param>
        /// <param name="start">The start time</param>
        /// <param name="end">The end time</param>
        /// <param name="location">The location</param>
        /// <param name="instructor">The uid of the professor</param>
        /// <returns>A JSON object containing {success = true/false}. 
        /// false if another class occupies the same location during any time 
        /// within the start-end range in the same semester, or if there is already
        /// a Class offering of the same Course in the same Semester,
        /// true otherwise.</returns>
        public IActionResult CreateClass(string subject, int number, string season, int year, DateTime start, DateTime end, string location, string instructor)
        {
            try
            {
                TimeOnly startTime = new TimeOnly(start.Hour, start.Minute, start.Second);
                TimeOnly endTime = new TimeOnly(end.Hour, end.Minute, end.Second);
                // Check if the create class using the same location that other class already used
                bool checkLocation = db.Classes.Any(c => c.Location == location &&
                                                    c.Start == startTime && c.End == endTime
                                                    && c.Season == season && c.Year == year);
                if (checkLocation)
                {
                    return Content("the location is used! chose another location");
                }

                // check if there is already a Class offering of the same Course in the same Semester
                bool checkClass = db.Classes.Any(c => c.Course.Depart.Subject == subject &&
                                                c.Course.Number == number && c.Season == season && c.Year == year);
                if (checkClass)
                {
                    return Content("this course got a class already");
                }

                var checkCourse = db.Courses.FirstOrDefault(c => c.Depart.Subject == subject && c.Number == number);
                if (checkCourse == null)
                {
                    return Content("the course can not be null here");
                }

                Class newClass = new Class
                {
                    Year = (ushort)year,
                    Season = season,
                    Location = location,
                    Start = startTime,
                    End = endTime,
                    CourseId = checkCourse.CourseId,
                    UId = instructor
                };

                db.Classes.Add(newClass);
                db.SaveChanges();

                return Json(new { success = true });
            }
            catch (Exception ex)
            {
                return Json(new { success = false, message = ex.Message });
            }
        }


        /*******End code to modify********/

    }
}

